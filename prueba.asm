LIST	P=18F8722
#INCLUDE		<P18F8722.INC>
    #DEFINE      LCD         PORTA,2                     ; 1 = Habilita el LCD   / 0 = Deshabilita el LCD
    #DEFINE      RST_LCD     PORTF,6                     ; 1 = No reinicia       / 0 = Reinicia
    #DEFINE      LCD_RS      VALA,7                      ; 1 = Envia dato al LCD / 0 = Envia instruccion al LCD
    #DEFINE      LCD_EN      VALA,6                      ; 1 = Prende el LCD     / 0 = Apaga el LCD
CONT	EQU		0X21
CONT2	EQU		0X22
TMP_TX	EQU		0X23
STATE	EQU		0X24
DATO	EQU		0X25

	CONFIG	CP0=OFF
	CONFIG	OSC=HS
	CONFIG	WDT=OFF
	CONFIG	MCLRE=OFF
	CONFIG	LVP=ON
	CONFIG	PWRT=OFF

			ORG		0X00
			CALL	CONF
			GOTO	MAIN
			ORG		0X08

INT_CALL
			BCF		INTCON,7
			BTFSS	PIR1,5
			GOTO	INT_END
			MOVF	RCREG1,W
			MOVWF	TMP_TX
			MOVWF	PORTD
			MOVLW	'0'
			SUBWF	TMP_TX
			BTFSS	TMP_TX,0
			GOTO	CLEAR
			MOVLW	0XFF
			MOVWF	PORTD
			MOVWF	STATE
			CALL	ESCRIBIR
			CALL	ESCRIBIR_ON
			BCF		PIR1,5
			
INT_END
			RETFIE

CLEAR
			CALL	ESCRIBIR_OFF
			CLRF	PORTD
			CLRF	STATE
			CALL	ESCRIBIR
			BCF		PIR1,5
			RETFIE
CONF
			BANKSEL	BAUDCON
			CLRF	BAUDCON
			BANKSEL	TRISD
			MOVLW	0XFF
			MOVWF	TRISB
			CLRF	TRISC
			BSF		TRISG,2
			BSF		TRISC,7
			CLRF	TRISD
			MOVLW	B'00100000'
			MOVWF	PIE1
			MOVLW	B'01000000'
			MOVWF	INTCON
			MOVLW	D'64'
			MOVWF	SPBRG
			MOVLW	B'00100100'
			MOVWF	TXSTA
			BANKSEL	PORTA
			MOVLW	B'10000000'
			MOVWF	RCSTA1

			CLRF	TMP_TX
			BSF		INTCON,7
			BSF		RCSTA1,4

			BANKSEL	TRISA
			BCF		TRISA,2
			BCF		TRISF,6
			BANKSEL	TRISC
			BCF		TRISC,3
			BCF		TRISC,5
			BANKSEL	PORTC
			CLRF	PORTC
			CLRF	PORTD
			BSF     LCD                                          ; habilita el display LCD
      		BCF     RST_LCD                                      ; reinicia el display LCD
      		CALL    DELAY                                    ; llama a LIT_DELAY
      		BSF     RST_LCD                                      ; no reinicia el LCD
      		CALL    CONFIG_SPI                                   ; configura la comunicacion serial para el LCD
      		CALL    INI_LCD
			BSF		LCD_EN 
			BSF		LCD_RS  
			RETURN


			
MAIN
			BSF     LCD_EN                                       ; LCD_EN = 1 / prende el display LCD
      		BCF     LCD_RS                                       ; LCD_RS = 0 / indica que se va a enviar una instruccion
      		MOVLW   b'00000001'                                  ; se carga la instruccion que limpia el LCD
      		CALL    PRINT                                        ; envia la instruccion
      		BSF     LCD_RS 
			GOTO	MAIN


DELAY: 
			MOVLW	d'10'	;Asigna el valor 7 a W
			MOVWF	CONT2	;Asigna el valor de W a CONT2
			MOVLW 	d'250'	;Asigna a W el valor 250
			MOVWF	CONT	;Asigna el valor de W a CONT
			NOP
			DECFSZ	CONT
			GOTO 	$-.2	;Vuelve a la linea del NOP
			DECFSZ	CONT2
			BRA    	$-.6	;Vuelve donde se asigna el valor de CONT2, para resetearlo
			RETURN


ESCRIBIR:
			BCF		INTCON,GIE
			BANKSEL	EEADR
			MOVLW	0X11
			MOVWF	EEADR
			MOVLW	STATE
			MOVWF	EEDATA
			BANKSEL	EECON1
			BCF		EECON1,EEPGD
			BSF		EECON1,WREN
			MOVLW	55H
			MOVWF	EECON2
			MOVLW	0XAA
			MOVWF	EECON2
			BSF		EECON1,WR
			BTFSC	EECON1,WR
			BRA		$-1
			BCF		EECON1,WREN
			BANKSEL	0
			RETURN

LEER:
			BANKSEL	EEADR
			MOVLW	0X11
			MOVWF	EEADR
			BANKSEL	EECON1
			BCF		EECON1,EEPGD
			BSF		EECON1,RD
			BANKSEL	EEDATA
			MOVF	EEDATA,W
			BANKSEL	0
			RETURN

ESCRIBIR_ON:
			BSF     LCD_EN                                       ; LCD_EN = 1 / prende el display LCD
      		BCF     LCD_RS                                       ; LCD_RS = 0 / indica que se va a enviar una instruccion
      		MOVLW   b'00000001'                                  ; se carga la instruccion que limpia el LCD
      		CALL    PRINT                                        ; envia la instruccion
      		BSF     LCD_RS                                   
			MOVLW	'O'
			CALL	PRINT
			MOVLW	'N'
			CALL	PRINT
			RETURN

ESCRIBIR_OFF:
		    BSF     LCD_EN                                       ; LCD_EN = 1 / prende el display LCD
      		BCF     LCD_RS                                       ; LCD_RS = 0 / indica que se va a enviar una instruccion
      		MOVLW   b'00000001'                                  ; se carga la instruccion que limpia el LCD
      		CALL    PRINT                                        ; envia la instruccion
      		BSF     LCD_RS 
			MOVLW	'O'
			CALL	PRINT
			MOVLW	'F'
			CALL	PRINT
			MOVLW	'F'
			CALL	PRINT
			RETURN
			

			#INCLUDE <Lib_LCD_18F.INC>
			END