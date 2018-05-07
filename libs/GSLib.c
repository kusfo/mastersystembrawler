
#include "GSLib.h"

#define GSL_GENERAL_RAM                 0xC000       ; location of general purpose ram
#define GSL_HORIZONTAL_ADDITION         0x0F         ; addition value required to push scrolltable pointer to right size of screen 
#define GSL_NAMETABLE_BASE_ADDRESS      0x7800       ; base address of nametable in vram including write bit set (14) 
#define GSL_NAMETABLE_HIGH_BYTE_START   0x78
#define GSL_NAMETABLE_HIGH_BYTE_END     0x7F  
#define GSL_NAMETABLE_WIDTH_IN_BYTES    0x40
#define VDP_CONTROL_PORT				0xBF
#define VDP_DATA_PORT					0xBE

// == RAM USAGE 
// Following is stored in scrolltable header.
#define GSL_ScrolltableSize             #_GSL_RAM + 0  // dw
#define GSL_WidthInMetatiles            #_GSL_RAM + 2  // dw          
#define GSL_HeightInMetatiles           #_GSL_RAM + 4  // dw
#define GSL_WidthInPixels               #_GSL_RAM + 6  // dw
#define GSL_HeightInPixels              #_GSL_RAM + 8  // dw          
#define GSL_VerticalAddition            #_GSL_RAM + 10 // dw          ; addition value required to push scrolltable point to bottom left.
#define GSL_OptionByte                  #_GSL_RAM + 12 // db

// Following is internal working ram
#define GSL_Scrolltable                 #_GSL_RAM + 13 // dw     
#define GSL_ScrolltableCurrentPosition  #_GSL_RAM + 15 // dw          ; top-left position of current screen in scrolltable
#define GSL_NametableCurrentPosition    #_GSL_RAM + 17 // dw          ; top-left position of current screen on vdp nametable
#define GSL_MetatileTable               #_GSL_RAM + 19 // dw
#define GSL_MetatileLookupBuffer        #_GSL_RAM + 21 // dw
#define GSL_MetatileXBuffer             #_GSL_RAM + 23 // dw
#define GSL_MetatileYBuffer             #_GSL_RAM + 25 // dw
#define GSL_X                           #_GSL_RAM + 27 // dw          ; current x point in scrolltable
#define GSL_Y                           #_GSL_RAM + 29 // dw          ; current y point in scroll table
#define GSL_Y224                        #_GSL_RAM + 31 // db          ; stores Y%224. Needed to speed things up.
#define GSL_YRefresh                    #_GSL_RAM + 32 // db          ; used in refresh window call for speed
#define GSL_XUpdateRequest              #_GSL_RAM + 33 // db          ; requested x adjustment (signed)
#define GSL_YUpdateRequest              #_GSL_RAM + 34 // db          ; requested y adjustment (signed)
#define GSL_XUpdateStatus               #_GSL_RAM + 35 // db          ; represents required updates if any
#define GSL_YUpdateStatus               #_GSL_RAM + 36 // db          ; ^^
#define GSL_NametableUpdateColumn       #_GSL_RAM + 37 // dw          ; first entry in nametable for column update
#define GSL_NametableUpdateRow          #_GSL_RAM + 39 // dw          ; first entry in nametable for row update
#define GSL_MetatileUpdatesCount        #_GSL_RAM + 41 // db
#define GSL_MetatileUpdatesAddress      #_GSL_RAM + 42 // dw
#define GSL_CollisionCount              #_GSL_RAM + 44 // db
#define GSL_Offset                      #_GSL_RAM + 45 //
#define GSL_NATColumnBuffer             #_GSL_RAM + 47 // dsb 64
#define GSL_NATRowBuffer                #_GSL_RAM + 111// dsb 72  
#define GSL_MetatileUpdates             #_GSL_RAM + 183// dsb 80      ; you need 10 bytes per maximum update, default here is 80 = 8 update!

// #GSL_VerticalAddition = *((unsigned int*)(&scrolltable + 5));
unsigned char GSL_RAM[263];


#pragma disable_warning 85 
#pragma disable_warning 59

void GSL_positionWindow(unsigned int X, unsigned int Y)
{
__asm
							pop de
							pop hl
							pop bc
							push bc
							push hl
							push de
							
							call GSL_PositionWindow

__endasm;
}							
			

			
unsigned int * GSL_metatileLookup(unsigned int x, unsigned int y)
{
__asm
							pop de
							pop hl
							pop bc
							push bc
							push hl
							push de
							
							call GSL_MetatileLookup
__endasm;
}



void GSL_tileLookup(unsigned int x, unsigned int y)
{
__asm

							pop de
							pop hl
							pop bc
							push bc
							push hl
							push de
							
							call GSL_TileLookup
__endasm;
}



void GSL_refreshVDP()
{
__asm
							push iy
							call GSL_refreshVDP
							pop iy
__endasm;
}



void GSL_scroll(char x, char y)
{
__asm
							pop bc
							pop hl
							push hl
							push bc
							
							push iy
							
							ld a, h
							ld (#GSL_YUpdateRequest), a
							ld a, l
							ld (#GSL_XUpdateRequest), a
							
							call GSL_ActiveDisplayRoutine
							pop iy
							
__endasm;
}



void GSL_VBlank()
{
__asm
							push iy
							call GSL_VBlankRoutine
							pop iy
__endasm;
}



void GSL_metatileUpdate()
{
__asm
							call GSL_MetatileUpdate
__endasm;
}



void GSL_metatileUpdateCustom(unsigned int x, unsigned int y, unsigned int offset)
{
__asm
							pop bc
							pop hl 
							ld (#GSL_MetatileXBuffer), hl
							pop hl
							ld (#GSL_MetatileYBuffer), hl
							pop hl
							ld de, (#GSL_Scrolltable)
							add hl, de
							ld (#GSL_MetatileLookupBuffer), hl
							push hl
							push hl
							push hl
							push bc
							
							call GSL_MetatileUpdate
__endasm;	
}


unsigned int GSL_getMapWidthInPixels()
{
	return *((unsigned int*)(&GSL_RAM + 6));
}



unsigned int GSL_getMapHeightInPixels()
{
	return *((unsigned int*)(&GSL_RAM + 8));
}



unsigned int GSL_getMapWidthInMetatiles()
{
	return *((unsigned int*)(&GSL_RAM + 2));
}



unsigned int GSL_getMapHeightInMetatiles()
{
	return *((unsigned int*)(&GSL_RAM + 4));
}



unsigned int GSL_getScrolltableSize()
{
	return *((unsigned int*)(&GSL_RAM + 0));
}



unsigned int GSL_getCurrentX()
{
	return *((unsigned int*)(&GSL_RAM + 27));
}



unsigned int GSL_getCurrentY()
{
	return *((unsigned int*)(&GSL_RAM + 29));
}



unsigned char GSL_getCollisionCount()
{
	return *((unsigned char*)(&GSL_RAM + 44));
}


unsigned char * GSL_getScrolltableAddress()
{
	return *(unsigned char**)(&GSL_RAM + 13);
}



void GSL_initializeMap(void *scrolltable, void *metatiles)
{

__asm						pop de
							pop hl
							pop bc
							push bc
							push hl
							push de
							
							call GSL_InitialiseMap
							
							jp _Finalise
							
							
							
							
							
; ===============================================================================
; GSLib 1.0 ASM

/*
 * == Initializes GSLlib for rendering of new map
 * @in hl: Scrolltable Location
 * @in bc: Metatile Location
 */
GSL_InitialiseMap:          ; == Store MetatileTable Address
                            ld (#GSL_MetatileTable), bc
                            
                            ; == Store CollisionCount (3rd byte in metatile table)
                            inc bc
                            inc bc
                            inc bc
                            ld a, (bc)
                            ld (#GSL_CollisionCount), a

                            ; == Copy Scrolltable Header to Ram
                            ld de, #_GSL_RAM
                            ld bc, #13
                            ldir
                            
                            ; == Store #GSL_Scrolltable (ldir will have pushed past header to actual table)
                            ld (#GSL_Scrolltable), hl
                            
                            ; == Initialize Ram
                            ld hl, #GSL_MetatileUpdates
                            ld (#GSL_MetatileUpdatesAddress), hl
                            xor a
                            ld (#GSL_MetatileUpdatesCount), a
                            ld (#GSL_XUpdateRequest), a
                            ld (#GSL_YUpdateRequest), a

                            ret
                            
                            
                            
/* == Sets Internal Settings for Specified Window Position
 * 
 * - Stores X,Y
 * - Creates Pointers based on X,Y
 * 
 * @in hl: X
 * @in bc: Y 
 */                                
GSL_PositionWindow:         ; == Store (x, y) in ram
                            ld (#GSL_Y), bc
                            ld (#GSL_X), hl
                            
                            
                            ; **** Calculate and Store #GSL_ScrolltableCurrentPosition
                            call GSL_MetatileLookup
                            
                            ; == Store #GSL_ScrolltableCurrentPosition
                            ld (#GSL_ScrolltableCurrentPosition), hl
                            
                            
                            ; *** Calculate and Store #GSL_Y224
                            ; == Y Modulo 224 (height of window in pixels)
                            ld hl, (#GSL_Y)
                            ld e, #224                
                            ld b, #8
_DivLoop:                   adc hl, hl
                            ld a, h
                            jr c, _DivCarry1
                            cp e
                            jr c, _DivCarry2
_DivCarry1:                 sub e
                            ld h, a
                            or a
_DivCarry2:                 djnz _DivLoop
                            ld a, l
                            rla
                            cpl
                            
                            ; == Store #GSL_Y224
                            ld a, h
                            ld (#GSL_Y224), a
                            
                            
                            ; *** Calculate and Store #GSL_NametableCurrentPosition (top left nametable entry on VDP)
                            ; == Use Previous Modulo to Calculate Namtable Row Address ((modulo & %11110000) * 4)
                            ld a, #248
                            and h
                            ld l, a
                            xor a
                            ld h, a
                            ld d, a
                            add hl, hl
                            add hl, hl
                            add hl, hl
                            
                            ; == Add X worth of nametable entries.
                            ; Divide by 8 then << 1 (below is optimization of this)
                            ld a, (#GSL_X)
                            and #248 ; %11111000
                            rra
                            rra
                            ld e, a
                            add hl, de
                            
                            ; == Add vdp base address of nametable 
                            ld de, #GSL_NAMETABLE_BASE_ADDRESS       
                            add hl, de
                            
                            ; == Store #GSL_NametableCurrentPosition
                            inc hl
                            inc hl
                            ld (#GSL_NametableCurrentPosition), hl   
                            
                            ; == Set VDP Fine Scroll Values
                            ld a, (#GSL_X)
                            out (#VDP_CONTROL_PORT), a
                            ld a, #136 ; $88
                            out (#VDP_CONTROL_PORT), a
                            
                            ld a, (#GSL_Y224)
                            out (#VDP_CONTROL_PORT), a
                            ld a, #137 ; $89
                            out (#VDP_CONTROL_PORT), a
                            
                            ret
                            
                            
                            
/* == Retreives from Scrolltable, Metatile ID and address
 * 
 * - Stores address in buffer #GSL_MetatileLookupBuffer.
 * - Requires GSL_MetatileLUT to have been created.
 * - GSL_MetatileLUT has Scrolltable base address baked in so no need do add this!
 * 
 * @in bc: Y
 * @in hl: X
 * 
 * @out hl: address of metatile in scrolltable
 * 
 */                            
GSL_MetatileLookup:         ; == Convert Y to LUT Entry and retrieve value
                            ld (#GSL_MetatileXBuffer), hl
                            ld (#GSL_MetatileYBuffer), bc
                            
                            ; == Divide Y by 16 to get Metatile Y Index
                            ld a, c
                            srl b
                            rra
                            srl b
                            rra
                            srl b
                            rra
                            srl b
                            rra
                            ld c, a
                            
                            ; == Multiply Metatile Y Index By Map Metatile Width
                            ld de, (#GSL_WidthInMetatiles)
                            ld hl, #0
                            ld a, b
                            ld b, #16
                            
_Mult16Loop:                add hl, hl
                            sla c
                            rla
                            jr nc, _Mult16NoAdd
                            add hl, de
_Mult16NoAdd:               djnz _Mult16Loop
                          
                            ; == Divide X by 16 to get Metatile X Index and add
                            ld de, (#GSL_MetatileXBuffer)
                            ld a, e
                            srl d
                            rra
                            srl d
                            rra
                            srl d
                            rra
                            srl d
                            rra
                            ld e, a
                            add hl, de
                            
                            ; == Add #GSL_MetatileTable Base Address
                            ld de, (#GSL_Scrolltable)
                            add hl, de
                            
                            ; == Put Metatile ID in a and store address in buffer.
                            ld (#GSL_MetatileLookupBuffer), hl
                            
                            ret        
                            
                            
                            
                                                        
/* == Retreives from Scrolltable, Nametable Entry
 * 
 * - Stores address of surrounding metatile in buffer #GSL_MetatileLookupBuffer.
 * - Requires GSL_MetatileLUT to have been created.
 * - GSL_MetatileLUT has Scrolltable base address baked in so no need do add this!
 * 
 *
 * @in hl: X
 * @in bc: Y
 * 
 * @out hl: Nametable entry
 */                      
GSL_TileLookup:             ; == Create offset for within Metatile (which nametable entry does x,y point to)
                            ld a, #8 ; %00001000
                            and l
                            rra
                            rra
                            ld d, a
                            ld a, #8 ; %00001000
                            and c
                            rra
                            or d
                            push af
                            
                            ; == Retrieve Metatile ID
                            call GSL_MetatileLookup
                            
                            ; == Resolve in to Metatile Address - ((ld << 3) + Offset + GSL_METATILE_TABLE)
                            ld a, (hl)
                            ld l, a                            
                            and #7 ; %111                          
                            ld h, a
                            ld a, l
                            and #248 ; %11111000
                            ld l, a
                            pop af
                            or l     
                            ld l, a                             ; hl = pointer to metatile nametable entry we need to write to vdp
                            ld de, (#GSL_MetatileTable)
                            add hl, de
                            
                            ; == Get Nametable entry using resolved address
                            ld a, (hl)                      ; indirection      
                            inc hl
                            ld h, (hl)
                            ld l, a
                            
                            ret   
                            
                            
                            
                            
/* == Updates Nametable contents (based on current internal window possition)
 * 
 * - Active Display Safe
 *
 * TODO:
 * - Optimizations are possible.
 */                             
GSL_refreshVDP:          	; == Set VDP Nametable Pointer
                            ld hl, (#GSL_NametableCurrentPosition)
                            ld (#GSL_NametableUpdateRow), hl

                            
                            ; == Keep a Modified Y to reflect rows being updated
                            ld hl, (#GSL_Y)
                            ld (#GSL_YRefresh), hl ; using this as temp ram entry
                            
                            ld a, (#GSL_Y224)
                            srl a
                            srl a
                            srl a
                            sub #28
                            ld (#GSL_NATColumnBuffer), a
                            
                            ld a, #28
                            ld (#GSL_NATColumnBuffer + 1), a
                            
                            
_RefreshLoop:               ; == Set VDP Pointer at Start of Row
                            ld hl, (#GSL_NametableUpdateRow)
                            ld c, #VDP_CONTROL_PORT
                            out (c), l
                            out (c), h
                            
                            ; == Lookup Metatile for this Row
                            ld bc, (#GSL_YRefresh)
                            ld hl, (#GSL_X)
                            call GSL_MetatileLookup
                            
                            ; == Store address in iy
                            push hl
                            pop iy
                            
                            ; == Put Y Offset in to ixl and Unroll a row of Nametable data.
                            ld a, (#GSL_YRefresh)
                            and #8 ; %00001000
                            rrca
                            ld (#GSL_Offset), a
                            call _unrollRowForNTUpdate          ; Unroll buffer table column for nametable updates!

							; == Add Offset to Start location in Buffer.
                            ld a, (#GSL_X)
                            rrca
                            rrca
                            and #2
                            add a, #2
                            ld hl, #GSL_NATRowBuffer
                            ld e, a
                            xor a
                            ld d, a
                            add hl, de
                            
                            ; == Calculate Number of Bytes to be Written in first Write, store in b.
                            ld a, (#GSL_X)
                            add a, #8
                            and #248 ; %11111000
                            rrca
                            rrca
                            ld d, a
                            ld a, #64
                            sub d
                            and #63 ; %00111111
                            ld d, a
                            
                            ; == Write Buffered Data to VDP Nametable
                            ld c, #VDP_DATA_PORT
_WriteRow1Loop:             outi
                            dec d
                            jp nz, _WriteRow1Loop
                            
                            ; == Set VDP Address to Start of Row for Second Write
                            ld de, (#GSL_NametableUpdateRow)
                            ld c, #VDP_CONTROL_PORT
                            ld a, #192 ; %11000000
                            and e
                            out (c), a
                            out (c), d
                            
                            ; == Calculate Number of Bytes to be Written in scond Write, store in b.
                            ld a, (#GSL_X)
                            add a, #8
                            and #248 ; %11111000
                            rrca
                            rrca
                            ld d, a
                            ld c, #VDP_DATA_PORT
                            
                            ; == Write Buffered Data to VDP Nametable
_WriteRow2Loop:             outi
                            dec d
                            jp nz, _WriteRow2Loop
                            
                            ; **** Prep for Next Write.
                            ; == Push VDP Nametable address to next Row.
                            ld hl, (#GSL_NametableUpdateRow)
                            ld de, #GSL_NAMETABLE_WIDTH_IN_BYTES
                            add hl, de
                            ld (#GSL_NametableUpdateRow), hl
                            
                            
                            ; == Have we written rows? Return if true;
                            ld a, (#GSL_NATColumnBuffer + 1)
                            dec a
                            ret z
                            ld (#GSL_NATColumnBuffer + 1), a
                            
                            ; == Is next row at top of VDP Nametable, reset Address if true;
                            ld a, (#GSL_NATColumnBuffer)
                            inc a
                            ld (#GSL_NATColumnBuffer), a
                            jp nz, _AdjustTempYValue
                            ld hl, (#GSL_NametableUpdateRow)
                            ld h, #GSL_NAMETABLE_HIGH_BYTE_START
                            ld a, #63 ; %00111111
                            and l
                            ld l, a
                            ld (#GSL_NametableUpdateRow), hl
                            
                            
                            ; == Adjust Temp Y Value for Next Write.
_AdjustTempYValue:          ld hl, (#GSL_YRefresh)
                            ld d, #0
                            ld e, #8
                            add hl, de
                            ld (#GSL_YRefresh), hl
                            
                            jp _RefreshLoop




/* == Update user defined Metatile Entry to VDP.
* This is used in cases where user knows the x,y, and array index of metatile to update.
* Useful for animation type stuff.
* NOTE: this is a more advanced feature that puts the onus on the coder to manage properly.
* 
* @in hl: Y
* @in de: X
* 
* @in bc: Srolltable Array Offset (pointing at entry to be updated)
*/
 
GSL_MetatileUpdateSpecific: ; == Populate Buffered Values with Custom Values.
                            ld (#GSL_MetatileYBuffer), hl
                            ld (#GSL_MetatileXBuffer), de
                            
                            ; == Add Array offset to Scrolltable bass address then store results.
                            ld hl, (#GSL_Scrolltable)
                            add hl, bc
                            ld (#GSL_MetatileLookupBuffer), hl
                            
                            ; == Flow to GSL_MetatileUpdate Below 
                            
                          
                          
                          
/* == Write last accessed Scrolltable Metatile entry to VDP.
 * 
 * NOTE: This Routine assumes that the scrolltable entry in question is in current display.
 * If you call this on an entry that is not in the current window space it will update
 * the metatile relative to current display creating garbage
 * 
 * In Short... ONLY CALL THIS ROUTINE IF YOU ARE SURE THE SCROLLTABLE ENTRY IS CURRENTLY BEING
 * DISPLAYED IN FULL ON SCREEN!
 * 
 * - GSL_MetatileLookup stores buffer of last entry looked up, this routine uses this to update
 * last looked up entry.
 */                          
GSL_MetatileUpdate:         ; == Identify Column of update
                            ld hl, (#GSL_MetatileYBuffer)
                            ld e, #224                
                            ld b, #8
_MDivLoop:                  adc hl, hl
                            ld a, h
                            jr c, _MDivCarry1
                            cp e
                            jr c, _MDivCarry2
_MDivCarry1:                sub e
                            ld h, a
                            or a
_MDivCarry2:                djnz _MDivLoop
                            ld a, l
                            rla
                            cpl
                            
                            ld a, h
                            rra
                            rra
                            rra
                            rra
                            and #15
                            add a, #240
                            ld h, a
                            xor a
                            ld l, a
                            ld d, a             ; Prep for X
                            srl h
                            rr l
                            
                            ; == Isolate metatile location in X and add
                            ld a, (#GSL_MetatileXBuffer)
                            and #240 ; %11110000
                            rra
                            rra
                            ld e, a
                            add hl, de
                            
                            ; == Write Resolved Address of Metatile on VDP (including register high bits)
                            ex de, hl
                            ld hl, (#GSL_MetatileUpdatesAddress)
                            ld (hl), e
                            inc hl
                            ld (hl), d
                            inc hl
                            ex de, hl
                            
                            ; == Resolve Metatile Location from ID
                            ld hl, (#GSL_MetatileLookupBuffer)
                            ld l, (hl)
                            ld a, l
                            and #7 ; %111                          
                            ;or #GSL_METATILE_TABLE_HIGH_BYTE     ; add high bits of metatile address (table must be on 2k boundary)
                            ld h, a
                            ld a, l
                            and #248 ; %11111000
                            ld l, a   
                            ld bc, (#GSL_MetatileTable)
                            add hl, bc
                            
                            ; == Copy Contents of Metatile To Ram Buffer
                            ldi
                            ldi
                            ldi
                            ldi
                            ldi
                            ldi
                            ldi
                            ldi
                            
                            ; == Update Count and Address for more Metatile Updates if any 
                            ld (#GSL_MetatileUpdatesAddress), de
                            ld hl, #GSL_MetatileUpdatesCount
                            inc (hl)
                            
                            ret
                            
                            
                            
                            
/* == Populates Buffer for VBlank Updates
 * 
 * - Extracts row of nametable entries from scrolltable
 * - Stored in Buffer for Vblank routine
 * - Metatile ids in scrolltable are pre processed to speed the resolving of their address (which is very time consuming)
 *      * (metatile_id << 3) & 248) + ((metatile_id >> 5) & 7)
 * 
 * @in iy: scrolltable pointer pointing to first metatile in update
 * 
 */  
_unrollRowForNTUpdate:      ld de, #GSL_NATRowBuffer         
                            ld c, #17*#4   
                            
_unrollMetaTileRow:         ld l, 0 (iy)                        ; == Resolve metatile address, current metatile pointed to by iy
                            ld a, l                             ; optimized <<3 shift, bits are stored as '43210765' for speed
                            and #7 ; %111                          
                            ld h, a
                            ld a, l
                            and #248 ; %11111000
                            ld l, a
                            ld a, (#GSL_Offset)                 ; add X offset (in ixl) to get desired column in meta tile.
                            or l       
                            ld l, a                             ; hl = pointer to metatile nametable entry we need to write to vdp
                            push bc
                            ld bc, (#GSL_MetatileTable)
                            add hl, bc
                            pop bc
                            
                            ldi                                 ; == Write 2x nametable entries to buffer.
                            ldi                                 ; metatiles are 2 entries wide so l+=2 to move to second entry.
                            ldi
                            ldi
                            
                            xor a                               ; loop
                            or c
                            ret z
                            
                            inc iy                              ; Update Scrolltable Pointer
                            
                            jp _unrollMetaTileRow
                            
                            
                            
                            
/* == Populates Buffer for VBlank Updates
 * 
 * - Extracts column of nametable entries from scrolltable
 * - Stored in Buffer for Vblank routine
 * - Metatile ids in scrolltable are pre processed to speed the resolving of their address (which is very time consuming)
 *      * (metatile_id << 3) & 248) + ((metatile_id >> 5) & 7)
 * 
 * @in iy: scrolltable pointer pointing to first metatile in update
 * @in ixl: X offset, needed to identify which column in metatile to extract.
 */                   
_unrollColumnForNTUpdate:   ld de, #GSL_NATColumnBuffer         
                            ld c, #15*#4   
                            
_unrollMetaTileColumn:      ld l, 0 (iy)                      ; == Resolve metatile address, current metatile pointed to by iy
                            ld a, l                             ; optimized <<3 shift, bits are stored as '43210765' for speed
                            and #7 ; %111                            
                            ld h, a
                            ld a, l
                            and #248 ; %11111000
                            ld l, a
                            ld a, (#GSL_Offset)                 
                            or l                                
                            ld l, a                             ; hl = pointer to metatile nametable entry we need to write to vdp
                            push bc
                            ld bc, (#GSL_MetatileTable)
                            add hl, bc
                            pop bc
                            
                            ldi                                 ; == Write 2x nametable entries to buffer.
                            ldi                                 ; metatiles are 2 entries wide so l+=2 to move to second entry.
                            inc hl                   
                            inc hl
                            ldi
                            ldi
                            
                            xor a                               ; loop
                            or c
                            ret z
                            
                            ld hl, (#GSL_WidthInMetatiles)       ; Update Scrolltable Pointer
                            ex de, hl
                            add iy, de
                            ex de, hl
                            
                            jp _unrollMetaTileColumn                                  
                            
                            


/* == Routine to be Called During Active Display
 * 
 * - Checked XUpdateRequest + YUpdateRequest Status for updates
 * - Uses pointers to speed up nametable and scrolltable lookups, updates.
 * - Checks boundary crosses and updates pointers as required.
 * 
 * NOTES: Somewhat Optimized.
 */                              
GSL_ActiveDisplayRoutine:   ; == Initialize
                            ld iy, #_GSL_RAM
                            xor a
                            ld (#GSL_XUpdateStatus), a
                            ld (#GSL_YUpdateStatus), a
                            
                            
_xScroll:                   ; == Check Left / Right Scroll... 
                            ld a, (#GSL_XUpdateRequest)              
                            and a                               
                            jp z, _yScroll                      ; no value = no horizontal scroll
                            jp m, _leftScroll                   ; signed = left scroll.
                                  
                                     
_rightScroll:               ; == Update currentXScroll Value.
                            ld hl, (#GSL_X)                      ; add to current x value
                            ld b, l
                            add a, l
                            jr nc, _noRightHighByteCarry
                            inc h
_noRightHighByteCarry:      ld l, a
                            xor b
                            ld d, a                             ; store low byte different pre / post scroll (for boundary tests)
                            ld (#GSL_X), hl      
                                   

_rightNametableCheck:       ; == Check for NameTable Boundary Cross
                            and #248 ; %11111000
                            jp z, _yScroll
                            
                            ; == NameTable Bounary Crossed... Process
                            ld a, #1
                            ld (#GSL_XUpdateStatus), a
                            
                            ld a, (#GSL_NametableCurrentPosition)            ; Update NameTable Pointer
                            ld c, a                             ; (addition bound within low 6 bits!!!)
                            and #192 ; %11000000                       
                            ld b, a
                            ld a, c
                            add a, #2
                            and #63 ; %111111
                            or b
                            ld (#GSL_NametableCurrentPosition), a


_rightScrolltableCheck:     ; == Check for BufferTable Boundary Cross
                            ld a, d                                 
                            and #240 ; %11110000                    
                            jp z, _yScroll
                            
                            ; == BufferTable Boundary Crossed... Process
                            ld hl, (#GSL_ScrolltableCurrentPosition)          ;  Update buffer table pointer.
                            inc hl
                            ld (#GSL_ScrolltableCurrentPosition), hl

                            jp _yScroll

                                                    
_leftScroll:                ; == Update currentXScroll Value.
                            ld hl, (#GSL_X)                      ; add to current x value
                            ld b, l
                            add a, l
                            jr c, _noLeftHighByteCarry
                            dec h
_noLeftHighByteCarry:       ld l, a
                            xor b
                            ld d, a                             ; store low byte different pre / post scroll (for boundary tests)
                            ld (#GSL_X), hl             
                            
_leftNametableCheck:        ; == Check for NameTable Boundary Cross
                            and #248 ; %11111000
                            jp z, _yScroll
                            
                            ; == NameTable Bounary Crossed... Process
                            ld a, #129
                            ld (#GSL_XUpdateStatus), a
                            
                            ld a, (#GSL_NametableCurrentPosition)        ; Update name table pointer.
                            ld c, a                             ; (subtraction bound within low 6 bits!!!)
                            and #192 ; %11000000                       
                            ld b, a
                            ld a, c
                            sub #2
                            and #63 ; %111111
                            or b
                            ld (#GSL_NametableCurrentPosition), a
                            
                            
_leftScrolltableCheck:      ; == Check for BufferTable Boundary Cross
                            ld a, d                                 
                            and #240 ; %11110000                       
                            jp z, _yScroll
                            
                            ; == BufferTable Boundary Crossed... Process
                            ld hl, (#GSL_ScrolltableCurrentPosition)          ;  Update buffer table pointer.
                            dec hl
                            ld (#GSL_ScrolltableCurrentPosition), hl
                                

_yScroll:                   ; == Check Up / Down Scroll... 
                            ld a, (#GSL_YUpdateRequest)
                            and a                           
                            jp z, _processUpdateBuffers          ; no value = no vertical scroll
                            jp m, _upScroll                     ; signed = left scroll.


_downScroll:                ; == Update dummyYScroll Value (wraps around 224 NameTable height!)
                            ld b, a
                            ld a, (#GSL_Y224)
                            add a, b
                            cp #224
                            jp c, _downNoCrossBoundary
                            sub #224
_downNoCrossBoundary:       ld (#GSL_Y224), a
                            ld a, b
                            
                            ; == Update currentYScroll Value.
                            ld hl, (#GSL_Y)             ; add to current y value
                            ld b, l
                            add a, l
                            jr nc, _noDownHighByteCarry
                            inc h
_noDownHighByteCarry:       ld l, a
                            xor b
                            ld d, a                             ; store low byte different pre / post scroll (for boundary tests)
                            ld (#GSL_Y), hl             
                            
                            
_downNametableCheck:        ; == Check for NameTable Boundary Cross
                            and #248 ; %11111000
                            jp z, _processUpdateBuffers 
                            
                            ; == NameTable Bounary Crossed... Process
                            ld a, #1
                            ld (#GSL_YUpdateStatus), a
                                                
                            ld hl, (#GSL_NametableCurrentPosition)                  ; Update name table pointer.
                            ld bc, #64                           
                            add hl, bc
                            ld a, #GSL_NAMETABLE_HIGH_BYTE_END                                      ; TODO shift this to .defines for variable nat positioning
                            cp h
                            jp nz, _downNoResetNTHighByte
                            ld h, #GSL_NAMETABLE_HIGH_BYTE_START
_downNoResetNTHighByte:     ld (#GSL_NametableCurrentPosition), hl

_downScrolltableCheck:      ; == Check for BufferTable Boundary Cross
                            ld a, d                                 
                            and #240 ; %11110000                       ; !SMC metatile width bitmask
                            jp z, _processUpdateBuffers
                            
                            ; == BufferTable Boundary Crossed... Process
                            
                            ld hl, (#GSL_ScrolltableCurrentPosition)
                            ld bc, (#GSL_WidthInMetatiles)
                            add hl, bc
                            ld (#GSL_ScrolltableCurrentPosition), hl

                            jp _processUpdateBuffers
                            
                            
_upScroll:                  ; == Update dummyYScroll Value (wraps around 224 NameTable height!)
                            ld b, a
                            ld a, (#GSL_Y224)
                            add a, b
                            jp c, _upNoCrossBoundary
                            sub #32
_upNoCrossBoundary:         ld (#GSL_Y224), a
                            ld a, b
                            
                            ; == Update currentYScroll Value.
                            ld hl, (#GSL_Y)             ; add to current y value
                            ld b, l
                            add a, l
                            jr c, _noUpHighByteCarry
                            dec h
_noUpHighByteCarry:         ld l, a
                            xor b
                            ld d, a                             ; store low byte different pre / post scroll (for boundary tests)
                            ld (#GSL_Y), hl             
                            
_upNametableCheck:          ; == Check for NameTable Boundary Cross
                            ld a, d
                            and #248 ; %11111000
                            jp z, _processUpdateBuffers

                            ; == NameTable Bounary Crossed... Process
                            ld a, #129
                            ld (#GSL_YUpdateStatus), a
                            
                            ld hl, (#GSL_NametableCurrentPosition)                  ; Update name table pointer.
                            ld bc, #64
                            or a
                            sbc hl, bc
                            ld a, #GSL_NAMETABLE_HIGH_BYTE_START - 1
                            cp h
                            jp nz, _upNoResetNTHighByte
                            ld h, #GSL_NAMETABLE_HIGH_BYTE_END - 1
_upNoResetNTHighByte:       ld (#GSL_NametableCurrentPosition), hl

_upScrolltableCheck:        ; == Check for BufferTable Boundary Cross
                            ld a, d                                 
                            and #240 ; %11110000                       ; !SMC metatile width bitmask
                            jp z, _processUpdateBuffers
                            
                            ; == BufferTable Boundary Crossed... Process
                            
                            ld hl, (#GSL_ScrolltableCurrentPosition)
                            ld bc, (#GSL_WidthInMetatiles)
                            xor a
                            sbc hl, bc
                            ld (#GSL_ScrolltableCurrentPosition), hl

                            
_processUpdateBuffers:       
_updateX:                   ; == Left / Right Scroll Occur?
                            ld a, (#GSL_XUpdateStatus)
                            or a
                            jp z, _updateY
                            jp m, _updateBufferLeft
                            
_updateBufferRight:               ; == Create Adjusted btPointer for _unrollColumnForNTUpdate call
                            ld iy, (#GSL_ScrolltableCurrentPosition)
                            ld de, #16
                            add iy, de

                            ; == Create X offset for _unrollColumnForNTUpdate call
                            ld a, (#GSL_X)                    ; get x offset for meta tile. Writing column so need to adjust for this.
                            and #8 ; %00001000
                            rrca
                            rrca
                            ld (#GSL_Offset), a    ;ld ixl, a
                            
                            
                            ; == Unroll MetaTiles and get NameTable data for update
                            call _unrollColumnForNTUpdate   
                            
                            ; == Store Adjusted NameTable Address for Update.
                            ld a, (#GSL_NametableCurrentPosition)
                            ld c, a
                            and #192 ; %11000000                       ; update name table pointer.
                            ld b, a
                            ld a, c
                            sub #2
                            and #63 ; %111111
                            or b
                            ld (#GSL_NametableUpdateColumn), a
                            ld a, (#GSL_NametableCurrentPosition + 1)
                            ld (#GSL_NametableUpdateColumn+ 1), a    
                            
                            jp _updateY
                            
                     
_updateBufferLeft:          ; == NameTable Update Required?
                            rra
                            jp nc, _updateY
                            
                            ; == Create Adjusted btPointer for _unrollColumnForNTUpdate call
                            ld iy, (#GSL_ScrolltableCurrentPosition)
                            ld a, (#GSL_X) 
                            add a, #8
                            and #8 ; %00001000
                            jp nz, _createXOffset
                            inc iy
                            
                            ; == Create X offset for _unrollColumnForNTUpdate call
_createXOffset:             rrca
                            rrca
                            ld (#GSL_Offset), a    ;ld ixl, a

                            ; == Unroll MetaTiles and get NameTable data for update                        
                            call _unrollColumnForNTUpdate
                                 
                            ; == Store NameTable Address for Update (no adjustment required)
                            ld hl, (#GSL_NametableCurrentPosition)
                            ld (#GSL_NametableUpdateColumn), hl

                           
_updateY:                   ; == Left / Right Scroll Occur?
                            ld a, (#GSL_YUpdateStatus)
                            or a
                            ret z
                            jp m, _updateBufferUp
                           


_updateBufferDown:          ; == NameTable Update Required?
                            rra
                            ret nc

                            ; == Create Adjusted btPointer for _unrollColumnForNTUpdate call
                            ld iy, (#GSL_ScrolltableCurrentPosition)
                            ld de, (#GSL_VerticalAddition)
                            add iy, de
                            
                            ld de, (#GSL_WidthInMetatiles)
                            ld a, (#GSL_Y)              ; get x offset for meta tile. Writing column so need to adjust for this.
                            sub #8
                            and #8 ; %00001000
                            jp nz, _createYOffset
                            add iy, de
                            
_createYOffset:             rrca
                            ld (#GSL_Offset), a    ;ld ixl, a
                                        
                            ; == Unroll MetaTiles and get NameTable data for update                             
                            call _unrollRowForNTUpdate          
                            
                            
                            ; == Store Adjusted NameTable Address for Update.
                            ld hl, (#GSL_NametableCurrentPosition)
                            ld de, #64
                            or a
                            sbc hl, de
                            ld a, #GSL_NAMETABLE_HIGH_BYTE_START - 1
                            cp h
                            jp nz, _noYNametableWrap
                            ld h, #GSL_NAMETABLE_HIGH_BYTE_END - 1
_noYNametableWrap:          ld (#GSL_NametableUpdateRow), hl
                            
                            ret

_updateBufferUp:            ; == NameTable Update Required?
                            rrca
                            ret nc
                            
                            ; == Get btPointer for _unrollColumnForNTUpdate call
                            ld iy, (#GSL_ScrolltableCurrentPosition)
                            
                            ; == Create Y offset for _unrollColumnForNTUpdate call
                            ld a, (#GSL_Y)              ; get x offset for meta tile. Writing column so need to adjust for this.
                            and #8 ; %00001000
                            rrca
                            ld (#GSL_Offset), a    ;ld ixl, a
                            call _unrollRowForNTUpdate          ; Unroll buffer table column for nametable updates!
                            
                            ; == Store NameTable Address for Update (no adjustment required)
                            ld hl, (#GSL_NametableCurrentPosition)
                            ld (#GSL_NametableUpdateRow), hl
                            
                            ret
                            
                            
                            
                            
/* == Vblank call writes required updates to Nametable in vdp.
 * 
 * - Not Active Display Safe!
 */                             
GSL_VBlankRoutine:          ; == Check if row update required.
                            ld a, (#GSL_YUpdateStatus)
                            and a
                            jp z, _updateColumn
                            
_writeRow:                  ; == Put VDP Pointer Address in Shadow Register and Set VDP Pointer.
                            exx
                            ld hl, (#GSL_NametableUpdateRow)                  
                            ld c, #VDP_CONTROL_PORT
                            out (c), l
                            out (c), h
                            exx
                            ld c, #VDP_DATA_PORT

                            ; == Construct LUT address For First Write
                            ld a, (#GSL_X)              
                            and #248 ; %11111000
                            rrca
                            rrca
                            ld hl, #_GSL_LUTRowUpdate
                            add a, l
                            jp nc, _finaliseLUTRowAddress
                            inc h
_finaliseLUTRowAddress:     ld l, a
                            push hl                             ; Story copy on stack
                            
                            
                            ; == Extract Jump Location from LUT and store in IY
                            ; Jump is for unrolled VDP Write code
                            ld e, (hl)                          
                            inc hl
                            ld d, (hl)
                            ld iy, #_UnrolledRowWrites
                            add iy, de

                            ; == Add Offset to Start location in Buffer.
                            ld a, (#GSL_X)
                            rrca
                            rrca
                            and #2
                            add a, #2
                            ld hl, #GSL_NATRowBuffer
                            ld e, a
                            xor a
                            ld d, a
                            add hl, de
                            
                            call IYJump
                            
                            ; == Update VDP Pointer For Second Write
                            ; Second write always starts at left most side of Nametable so clear
                            ; low bytes of address and update.
                            exx
                            ld a, #192 ; %11000000
                            and l
                            out (c), a
                            out (c), h
                            exx
                            
                            ; == Move Forward 130 bytes in LUT
                            ex de, hl
                            pop hl
                            ld a, #64
                            add a, l
                            ld l, a
                            jp nc, _rowExtractJumpAddress
                            inc h
                            
_rowExtractJumpAddress:     ; == Extract Jump Location from LUT and store in IY
                            ; Jump is for unrolled VDP Write code
                            ld c, (hl)                          
                            inc hl
                            ld b, (hl)
                            ld iy, #_UnrolledRowWrites
                            add iy, bc
                            ex de, hl
                            ld c, #VDP_DATA_PORT
                            call IYJump
                            
_updateColumn:              ; == Check if row update required.
                            ld a, (#GSL_XUpdateStatus)
                            and a
                            jp z, _MetatileUpdates
                            
                            ; == Put VDP Pointer Address in Shadow Register and Set VDP Pointer.
                            exx
                            ld hl, (#GSL_NametableUpdateColumn)            
                            ld de, #64                           ; Each new column entry is 64 bytes ahead, store here for addition
                            ld c, #VDP_CONTROL_PORT
                            exx
                            ld c, #VDP_DATA_PORT
                            
                            ; == Construct LUT address For First Write
                            ld a, (#GSL_Y224)
                            and #248 ; %11111000
                            rrca
                            rrca
                            ld hl, #_GSL_LUTColumnUpdate
                            add a, l
                            jp nc, _finaliseLUTColumnAddress
                            inc h
_finaliseLUTColumnAddress:  ld l, a
                            push hl                             ; Story copy on stack
                            
                            ; == Extract Jump Location from LUT and store in IY
                            ; Jump is for unrolled VDP Write code
                            ld e, (hl)                          
                            inc hl
                            ld d, (hl)
                            ld iy, #_UnrolledColumnWrites
                            add iy, de
                            
                            ; == Add Offset to Start location in Buffer.
                            ld a, (#GSL_Y)
                            and #8 ; %00001000
                            rrca
                            rrca
                            ld hl, #GSL_NATColumnBuffer
                            ld e, a
                            xor a
                            ld d, a
                            add hl, de
                            
                            call IYJump
                            
                            ; == Update VDP Pointer For Second Write
                            ; Second write always starts at top row of Nametable so reset high byte 
                            ; to GSL_NAMETABLE_HIGH_BYTE_START, keep low bytes of address and update.
                            exx
                            ld a, (#GSL_NametableUpdateColumn)                  ; Get vram nametable pointer
                            and #63 ; %00111111
                            ld h, #GSL_NAMETABLE_HIGH_BYTE_START
                            ld l, a
                            exx

                            ; == Move Forward 114 bytes in LUT
                            ex de, hl
                            pop hl
                            ld a, #56
                            add a, l
                            ld l, a
                            jp nc, _columnExtractJumpAddress
                            inc h

_columnExtractJumpAddress:  ; == Extract Jump Location from LUT and store in IY
                            ; Jump is for unrolled VDP Write code
                            ld c, (hl)                          
                            inc hl
                            ld b, (hl)
                            ld iy, #_UnrolledColumnWrites
                            add iy, bc
                            ex de, hl
                            ld c, #VDP_DATA_PORT
                            call IYJump
                            
_MetatileUpdates:            ; == Check if Metatile Updates are Required.
                            ld a, (#GSL_MetatileUpdatesCount)
                            or a
                            jp z, _VBlankCleanup
                            
                            ld hl, #GSL_MetatileUpdates
                            ld b, a
                            
_MetatileUpdatesLoop:       ; == Process each metatile Update
                            ld a, #7                 ; count = number of metatile to update.
                            add a, b                ; add 7 to current number, outi will reduce this by 8
                            ld b, a                 ; end result will be to subtract 1 from current number 
                            
                            ; == First word is VDP address of update. Update VDP Pointer.
                            ld d, (hl)
                            inc hl
                            ld e, (hl)
                            inc hl
                            ld c, #VDP_CONTROL_PORT
                            out (c), d
                            out (c), e
                            
                            ; == Write first two entries of Metatile.
                            dec c
                            outi
                            outi
                            outi
                            outi
                            
                            ; == Update VDP pointer to be one nametable row lower.
                            ld a, #64 ; $40
                            add a, d
                            ld d, a
                            inc c
                            out (c), d
                            out (c), e
                            
                            ; == Write last two entries.
                            dec c
                            outi
                            outi
                            outi
                            outi
                            
                            ; == Loop if required.
                            ld a, b
                            or a
                            jp nz, _MetatileUpdatesLoop
                            
                            
_VBlankCleanup:             ; == Reset RAM Variables for next Active Display.
                            ld hl, #GSL_MetatileUpdates
                            ld (#GSL_MetatileUpdatesAddress), hl
                            
                            xor a
                            ld (#GSL_MetatileUpdatesCount), a
                            ld (#GSL_XUpdateRequest), a
                            ld (#GSL_YUpdateRequest), a
                            
                            ; == Update Screen X,Y Scroll
                            ld a, (#GSL_X)
                            neg
                            out (#VDP_CONTROL_PORT), a
                            ld a, #136 ; $88
                            out (#VDP_CONTROL_PORT), a
                            
                            ld a, (#GSL_Y224)
                            out (#VDP_CONTROL_PORT), a
                            ld a, #137 ; $89
                            out (#VDP_CONTROL_PORT), a 
                            
                            ret
                            
                            
IYJump:                     jp (iy) 
                            ret
                            
                        
/* == Unrolled VDP Row Write
 * Used to speed write rows of data on VBlank row update.
 */                            
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi   
                            outi
                            outi
                            
_UnrolledRowWrites:         ret




/* == Unrolled VDP Column Write
 * Used to speed write columns of data on VBlank row update.
 */ 
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
                            exx
                            out (c), l
                            out (c), h
                            add hl, de
                            exx
                            outi
                            outi
_UnrolledColumnWrites:      ret    


_Finalise:					

__endasm;
}



const unsigned int GSL_LUTColumnUpdate[] = 
{
	0xFECC, 0xFED7, 0xFEE2, 0xFEED, 0xFEF8, 0xFF03, 0xFF0E, 0xFF19, 0xFF24, 0xFF2F, 0xFF3A, 0xFF45, 0xFF50, 0xFF5B, 0xFF66, 0xFF71, 
	0xFF7C, 0xFF87, 0xFF92, 0xFF9D, 0xFFA8, 0xFFB3, 0xFFBE, 0xFFC9, 0xFFD4, 0xFFDF, 0xFFEA, 0xFFF5, 0x0000, 0xFFF5, 0xFFEA, 0xFFDF, 
	0xFFD4, 0xFFC9, 0xFFBE, 0xFFB3, 0xFFA8, 0xFF9D, 0xFF92, 0xFF87, 0xFF7C, 0xFF71, 0xFF66, 0xFF5B, 0xFF50, 0xFF45, 0xFF3A, 0xFF2F, 
	0xFF24, 0xFF19, 0xFF0E, 0xFF03, 0xFEF8, 0xFEED, 0xFEE2, 0xFED7
};

const unsigned int GSL_LUTRowUpdate[] = 
{
	0xFF84, 0xFF88, 0xFF8C, 0xFF90, 0xFF94, 0xFF98, 0xFF9C, 0xFFA0, 0xFFA4, 0xFFA8, 0xFFAC, 0xFFB0, 0xFFB4, 0xFFB8, 0xFFBC, 0xFFC0, 
	0xFFC4, 0xFFC8, 0xFFCC, 0xFFD0, 0xFFD4, 0xFFD8, 0xFFDC, 0xFFE0, 0xFFE4, 0xFFE8, 0xFFEC, 0xFFF0, 0xFFF4, 0xFFF8, 0xFFFC, 0x0000, 
	0xFFFC, 0xFFF8, 0xFFF4, 0xFFF0, 0xFFEC, 0xFFE8, 0xFFE4, 0xFFE0, 0xFFDC, 0xFFD8, 0xFFD4, 0xFFD0, 0xFFCC, 0xFFC8, 0xFFC4, 0xFFC0, 
	0xFFBC, 0xFFB8, 0xFFB4, 0xFFB0, 0xFFAC, 0xFFA8, 0xFFA4, 0xFFA0, 0xFF9C, 0xFF98, 0xFF94, 0xFF90, 0xFF8C, 0xFF88, 0xFF84, 0xFF80
};