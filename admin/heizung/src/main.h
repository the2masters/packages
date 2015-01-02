#include "hilfsfunktionen.h"

#define DATENSATZLEN 85

static const struct speicherbereich {
	const char addrhi, addrlo, len;
} speicherbereiche[] = {
	{ 0x08, 0x00, 28 },	// Bei Änderungen Datensatzgröße anpassen usw
	{ 0x08, 0x3A, 14 },
	{ 0x08, 0x83, 19 },
	{ 0x08, 0xA7, 8 },
	{ 0x55, 0x00, 10 },
	{ 0x55, 0x25, 4 },
	{ 0x00, 0xF8, 2 }
};

static const struct datenpunkt {
	const size_t offset;
	const char *name;
	size_t (*convert)(const char bytes[], char *string, size_t maxlen);
} datenpunkte[] = {
	{ 0, "temp_aussen_raw", convertTemp2 },
	{ 2, "temp_kessel_raw", convertTemp2 },
	{ 4, "temp_speicher_raw", convertTemp2 },
	{ 8, "temp_abgas_raw", convertTemp2 },
	{ 10, "temp_ruecklauf_raw", convertTemp2 },
	{ 12, "temp_vorlauf_raw", convertTemp2 },
	{ 14, "temp_aussen", convertTemp2 },
	{ 16, "temp_kessel", convertTemp2 },
	{ 18, "temp_speicher", convertTemp2 },
	{ 22, "temp_abgas", convertTemp2 },
	{ 24, "temp_ruecklauf", convertTemp2 },
	{ 26, "temp_vorlauf", convertTemp2 },

	{ 28, "status_aussen", convertBool },
	{ 29, "status_kessel", convertBool },
	{ 30, "status_speicher", convertBool },
	{ 32, "status_abgas", convertBool },
	{ 33, "status_ruecklauf", convertBool },
	{ 34, "status_vorlauf", convertBool },
	{ 36, "status_brenner", convertBool },
	{ 39, "status_warmwasserpumpe", convertBool },
	{ 40, "status_zirkulationspumpe", convertBool },
	{ 41, "status_sammelstoerung", convertBool },

	{ 42, "status_brennerstoerung", convertBool },
	{ 45, "zaehler_brennerbetriebsstunden", convertLong },
	{ 49, "zaehler_brennerstarts", convertLong },
	{ 53, "systemzeit", convertTime },

	{ 61, "zaehler_brennerbetriebsstunden1", convertLong },
	{ 65, "zaehler_brennerbetriebsstunden2", convertLong },

	{ 69, "temp_kessel_ist", convertTemp2 },
	{ 71, "temp_kessel_soll", convertTemp2 },
	{ 73, "temp_kessel_ein", convertTemp2 },
	{ 75, "temp_kessel_aus1", convertTemp2 },
	{ 77, "temp_kessel_aus2", convertTemp2 },

	{ 79, "temp_aussen1", convertTemp2 },
	{ 81, "temp_aussen2", convertTemp2 },
};
#define IDENTIFIKATIONBEI 83
#define IDENTIFIKATION ((int16_t)0x9820)


