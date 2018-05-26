#include "libs/SMSlib.h"
#include "gamelogic.h"

void main() {
	init_game();
    while (1) {
        play_game();
    }
}

SMS_EMBED_SEGA_ROM_HEADER(9999, 0); // code 9999 hopefully free, here this means 'homebrew'
SMS_EMBED_SDSC_HEADER_AUTO_DATE(0, 1, "1985Alternativo", "KatKills",
	"Punk Cat Brawler for SMS");