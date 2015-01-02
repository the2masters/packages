#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <endian.h>
#include <poll.h>
#include <time.h>
#define __ASSERT_USE_STDERR 1
#include <assert.h>
#include "main.h"

#define NAME "Heizungsplugin"
#define DEVICE "/dev/ttyUSB0"
#define READTIMEOUT_MS 1500

//sudo socat PTY,link=/dev/ttyUSB0,echo=0,raw tcp:192.168.201.2:1234
//sudo chmod 777 /dev/ttyUSB0
//LANG=c gcc -std=c99 -Wall main.c -o heizung
//http://openv.wikispaces.com/Adressen

void sendeanzabbix(const char *text)
{
	//FILE *zabbix = fopen("/tmp/zabbixfile", "w");
	FILE *zabbix = popen("zabbix_sender -c /etc/zabbix_agentd.conf -s $(cat /proc/sys/kernel/hostname) -i -", "w");
	if(zabbix == NULL) {
		perror(NAME ": popen");
		abort();
	}

	if(fputs(text, zabbix) == EOF) {
		fprintf(stderr, "konnte keine Daten an zabbix senden\n");
		abort();
	}
	pclose(zabbix);
}

int main(void)
{
	int fd = open(DEVICE, O_RDWR);
	if(fd == -1) {
		perror(NAME ": open(" DEVICE ")");
		abort();
	}

	struct termios termios;
	if(tcgetattr(fd, &termios))
		perror(NAME ": tcgetattr");
	cfmakeraw(&termios);
	if(cfsetspeed(&termios, B4800))
		perror(NAME ": cfsetspeed");
	termios.c_cflag &= ~(PARODD | CLOCAL | CRTSCTS);
	termios.c_cflag |= (CSTOPB | PARENB);
	if(tcsetattr(fd, TCSAFLUSH, &termios))
		perror(NAME ": tcsetattr");

	for(;;) {
		char byte;

		// Warte auf Start-Byte
		do {
			if(read(fd, &byte, 1) != 1) {
				perror(NAME ": read");
				abort();
			}
			if(byte != 0x05) {
				fprintf(stderr, NAME ": unerwartetes Byte empfangen: %02x\n", byte);
				sendeanzabbix("- heizung.err_communication 1");
			}
		}
		while (byte != 0x05);

		// Antworte (ACK)
		byte = 0x01;
		if(write(fd, &byte, 1) != 1) {
			perror(NAME ": write");
			abort();
		}

		// Timeout ermitteln
		struct timespec timeout, timenow;
		if(clock_gettime(CLOCK_MONOTONIC, &timenow)) {
			perror(NAME ": clock_gettime(CLOCK_MONOTONIC)");
			abort();
		}
		timeout = time_add(timenow, READTIMEOUT_MS);

		// Arbeite Liste der Speicherbereiche ab und speichere linear in datensatz
		char datensatz[DATENSATZLEN];
		char *readbufferpos = datensatz;
		for(const struct speicherbereich *speicherbereich = speicherbereiche; speicherbereich < speicherbereiche + ARRAYSIZE(speicherbereiche); speicherbereich++) {
			// Sende Anfrage auf einen Speicherbereich
			const char sendbuffer[] = {0xF7, speicherbereich->addrhi, speicherbereich->addrlo, speicherbereich->len};
			if(write(fd, sendbuffer, ARRAYSIZE(sendbuffer)) != ARRAYSIZE(sendbuffer)) {
				perror(NAME ": write");
				abort();
			}

			// Daten einlesen, teilweise kommen die Daten Byte-weise
			char *readbufferend = readbufferpos + speicherbereich->len;
			assert(readbufferend <= datensatz + ARRAYSIZE(datensatz));
			while(readbufferpos < readbufferend) {
				// Restzeit vom timeout berechnen
				if(clock_gettime(CLOCK_MONOTONIC, &timenow)) {
					perror(NAME ": clock_gettime(CLOCK_MONOTONIC)");
					abort();
				}
				struct timespec timeleft = time_diff(timenow, timeout);

				// Mit poll auf Daten unter berücksichtigung des Timeouts warten
				struct pollfd pollfd = { .fd = fd, .events = POLLIN };
				int ret = ppoll(&pollfd, 1, &timeleft, NULL);
				if(ret < 0) {
					perror(NAME ": poll");
					abort();
				}
				if(ret == 0) // Timeout
					break;

				ssize_t len = read(fd, readbufferpos, readbufferend - readbufferpos);
				if(len < 1) {
					perror(NAME ": read");
					abort();
				}
				readbufferpos += len;
			}
			// timeout?
			if(readbufferpos < readbufferend )
				break;
		}
		// timeout?
		if(readbufferpos < datensatz + ARRAYSIZE(datensatz) ) {
			fprintf(stderr, "Timeout, read %" PRIdPTR "/%" PRIdPTR " bytes\n", readbufferpos - datensatz, ARRAYSIZE(datensatz));
			sendeanzabbix("- heizung.err_timeout 1");
			continue;
		}
		// Stimmen empfangene Daten?
		if(b2s(datensatz + IDENTIFIKATIONBEI) != IDENTIFIKATION) {
			fprintf(stderr, "Fehlerhafte Daten, Identifikation der Heizung: %" PRIdFAST16 " statt %" PRId16 "\n", b2s(datensatz + IDENTIFIKATIONBEI), IDENTIFIKATION);
			sendeanzabbix("- heizung.err_communication 1");
			continue;
		}

		// Sende alles an zabbix
		char text[65536];
		char *textpos = text, *textende = text + ARRAYSIZE(text);
		for(const struct datenpunkt *datenpunkt = datenpunkte; datenpunkt < datenpunkte + ARRAYSIZE(datenpunkte); datenpunkt++) {
			textpos += snprintf(textpos, textende - textpos, "- heizung.%s ", datenpunkt->name);
			if(textpos >= textende) {
				fprintf(stderr, "Sendepuffer zu klein\n");
				abort();
			}
			textpos += datenpunkt->convert(datensatz + datenpunkt->offset, textpos, textende - textpos);
			if(textpos >= textende) {
				fprintf(stderr, "Sendepuffer zu klein\n");
				abort();
			}
			textpos += snprintf(textpos, textende - textpos, "\n");
			if(textpos >= textende) {
				fprintf(stderr, "Sendepuffer zu klein\n");
				abort();
			}

			//printf("%s: %s\n", datenpunkt->name, text);
		}
		textpos += snprintf(textpos, textende - textpos, "- heizung.err_communication 0\n- heizung.err_timeout 0\n");
		if(textpos >= textende) {
			fprintf(stderr, "Sendepuffer zu klein\n");
			abort();
		}


		sendeanzabbix(text);

		// Warte auf 8 Start-Bytes, um Pause von etwa 20 Sek zu simulieren (Start-Byte alle 2,25s)
		for(int i = 0; i < 8; i++)
			do {
				if(read(fd, &byte, 1) != 1) {
					perror(NAME ": read");
					abort();
				}
				if(byte != 0x05) {
					fprintf(stderr, NAME ": unerwartetes Byte empfangen: %02x\n", byte);
					sendeanzabbix("- heizung.err_communication 1");
				}
			}
			while (byte != 0x05);
	}

	close(fd);
	return EXIT_SUCCESS;
}

