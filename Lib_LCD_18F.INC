;===================================================================================================================
;                                 LIBRERIA LCD PARA PIC18 EXPLORER BOARD
;===================================================================================================================
;
;Se deben definir las siguientes variables para el correcto funcionamiento de la libreria:
;	#DEFINE	LCD	PORTA,2				;1= Habilita LCD / 0= Deshabilita LCD
;	#DEFINE	RST_LCD	PORTF,6				;1= No resetea / 0= Resetea
;	#DEFINE	LCD_RS	VALA,7				;1= Envia dato al LCD / 0= Envia Instruccion al LCD
;	#DEFINE	LCD_EN	VALA,6				;1= Prende LCD / 0= Apaga LCD
;Los siguientes pines se deben configurar de la siguiente manera:
;PORTA,2 = OUT
;PORTF,6 = OUT
;PORTC,3 = OUT
;PORTC,5 = OUT
;===================================================================================================================
; Incluir libreria para LCD al final del programa 	#INCLUDE	<Lib_LCD_18F.INC>
; Instrucciones para LCD
; CALL	PRINT 		/ Envia lo que esta en W al LCD
; CALL	CONFIG_SPI 	/ Inicializa el chip SPI
; CALL	INI_LCD		/ Inicializa el modulo LCD (Configuracion basica)
;	LCD			en 		1= Habilita LCD / 0= Deshabilita LCD
;	RST_LCD  	en		1= No resetea / 0= Resetea
;	LCD_RS 		en		1= Envia dato al LCD / 0= Envia Instruccion al LCD
;	LCD_EN 		en		1= Prende LCD / 0= Apaga LCD
;=====================================Inicio de la libreria============================================================
	LIST		P=18F8722		;Indica que PIC se utilizara
	CBLOCK						;Declaracion de variables
	VALA						;			"
	VALB						;			"
	ENDC						;Termina la declaracion de variables
;===============================================CONFIG_SPI==================================================
CONFIG_SPI
	BANKSEL	SSP1STAT			;Cambia de banco
	CLRF 	SSP1STAT 			;Limpia el SSP1STAT
	MOVLW 	B'01000000' 		;Pone el SPI en modo Maestro
	MOVWF 	SSP1STAT 			;cke = 0 (modo 1,1)
	BANKSEL	SSP1CON1			;Cambia de banco
	CLRF 	SSP1CON1 			;Limpia el SSP1CON
	MOVLW 	B'00100010' 		;Enciende el SPI
	MOVWF 	SSP1CON1 			;clk/16,ckp=1(modo 1,1)
;==========================================CONFIG_PORTA=====================================================
;Envia los datos a travez del puerto serial para configurar el puerto A del chip MCP23S17
;=============================================================================================================
	BCF		LCD					;Limpia LCD (PortA,2)
	MOVLW	0x40				;Cargas un 40 Hexa. a W
	MOVWF	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a Chek para revisar si ya se envio el dato
	MOVLW	0x00				;Mueve un 0 Hex. a W
	MOVWF	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a Chek para revisar si ya se envio el dato
	MOVLW	0x00				;Mueve un 0 Hex. a W
	MOVWF	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a Chek para revisar si ya se envio el dato
	BSF		LCD					;Setea LCD (PortA,2)
;==================================================CONFIG_PORTB=============================================
;Envia los datos a travez del puerto serial para configurar el puerto B del chip MCP23S17
;=============================================================================================================
	bcf		LCD					;Limpia LCD (PortA,2)
	movlw	0x40				;Cargas un 40 Hexa. a W
	movwf	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	call	CHEK				;Llama a Chek para revisar si ya se envio el dato
	movlw	0x01				;Mueve un 1 Hex. a W
	movwf	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	call	CHEK				;Llama a Chek para revisar si ya se envio el dato
	movlw	0x00				;Mueve un 0 Hex. a W
	movwf	SSP1BUF				;Envia lo que esta en W al Bufer del SPI
	call	CHEK				;Llama a Chek para revisar si ya se envio el dato
	bsf		LCD					;Setea LCD (PortA,2)
	RETURN						;Retorna a la siguiente linea del Call
;===================================================INICIALIZA LCD===============================================
;Configura el LCD como:8 Bits, N°de lineas=2, Caracter =5x7, cursor apagado,incrementa posicion de cursor y texto no se desplaza
;================================================================================================================
INI_LCD
	MOVLW	B'01111111'			;RS=0 /ENVIA INSTRUCCION Y E=1 /HABILITA EL MODULO LCD
	MOVWF	VALA				;Carga lo que esta en W a la variable VALA que representa el puerto A del MCP23S17
	MOVLW	B'00111000'			;Instruccion que se quiere enviar al LCD(bus de 8 Bits, N°de lineas=2, Caracter =5x7)
	MOVWF	VALB				;Carga lo que esta en W a la variable VALB que representa el puerto B del MCP23S17
	CALL	INSTRUCCION			;Llama a rutina instruccion qeu se encarga de enviar los datos por el SPI
	MOVLW	B'00001100'			;Instruccion que se quiere enviar al LCD(Pantalla encendida, Cursor apagado y sin intermitencia)
	MOVWF	VALB				;Carga lo que esta en W a la variable VALB que representa el puerto B del MCP23S17
	CALL	INSTRUCCION			;Llama a rutina instruccion qeu se encarga de enviar los datos por el SPI
	MOVLW	B'00000001'			;Instruccion que se quiere enviar al LCD(Limpia la pantalla y retorna a HOME)
	MOVWF	VALB				;Carga lo que esta en W a la variable VALB que representa el puerto B del MCP23S17
	CALL	INSTRUCCION			;Llama a rutina instruccion qeu se encarga de enviar los datos por el SPI
	MOVLW	B'00000110'			;Instruccion que se quiere enviar al LCD(Incrementa posicion de cursor y texto no se desplaza)
	MOVWF	VALB				;Carga lo que esta en W a la variable VALB que representa el puerto B del MCP23S17
	CALL	INSTRUCCION			;Llama a rutina instruccion qeu se encarga de enviar los datos por el SPI
	RETURN
;=============================================RUTINA PRINT======================================================
;Rutina que carga el contenido de W en la variable VALB para que esta sea enviada por el puerto serial (SPI)
;===============================================================================================================
PRINT
	MOVWF	VALB				;Carga lo que viene en W a la variable VALB que representa el puero B del MCP23S17
	CALL	INSTRUCCION			;Llama a INSTRUCCION (Rurina que se encarga de enviar los datos por el SPI)
	RETURN						;Retorna del LLamado
;=============================================PRINT_LETRA======================================================
;Rutina que carga una letra en W y la envia a  VALB para que esta sea enviada por el puerto serial (SPI)
;===============================================================================================================
PRINT_A
	MOVLW	'A'
	CALL	PRINT
	RETURN
PRINT_B
	MOVLW	'B'
	CALL	PRINT
	RETURN
PRINT_C
	MOVLW	'C'
	CALL	PRINT
	RETURN
PRINT_D
	MOVLW	'D'
	CALL	PRINT
	RETURN
PRINT_E
	MOVLW	'E'
	CALL	PRINT
	RETURN
PRINT_F
	MOVLW	'F'
	CALL	PRINT
	RETURN
PRINT_G
	MOVLW	'G'
	CALL	PRINT
	RETURN
PRINT_H
	MOVLW	'H'
	CALL	PRINT
	RETURN
PRINT_I
	MOVLW	'I'
	CALL	PRINT
	RETURN
PRINT_J
	MOVLW	'J'
	CALL	PRINT
	RETURN
PRINT_K
	MOVLW	'K'
	CALL	PRINT
	RETURN
PRINT_L
	MOVLW	'L'
	CALL	PRINT
	RETURN
PRINT_M
	MOVLW	'M'
	CALL	PRINT
	RETURN
PRINT_N
	MOVLW	'N'
	CALL	PRINT
	RETURN
PRINT_Ñ
	MOVLW	'Ñ'
	CALL	PRINT
	RETURN
PRINT_O
	MOVLW	'O'
	CALL	PRINT
	RETURN
PRINT_P
	MOVLW	'P'
	CALL	PRINT
	RETURN
PRINT_Q
	MOVLW	'Q'
	CALL	PRINT
	RETURN
PRINT_R
	MOVLW	'R'
	CALL	PRINT
	RETURN
PRINT_S
	MOVLW	'S'
	CALL	PRINT
	RETURN
PRINT_T
	MOVLW	'T'
	CALL	PRINT
	RETURN
PRINT_U
	MOVLW	'U'
	CALL	PRINT
	RETURN
PRINT_V
	MOVLW	'V'
	CALL	PRINT
	RETURN
PRINT_W
	MOVLW	'W'
	CALL	PRINT
	RETURN
PRINT_X
	MOVLW	'X'
	CALL	PRINT
	RETURN
PRINT_Y
	MOVLW	'Y'
	CALL	PRINT
	RETURN
PRINT_Z
	MOVLW	'Z'
	CALL	PRINT
	RETURN
PRINT_a
	MOVLW	'a'
	CALL	PRINT
	RETURN
PRINT_b
	MOVLW	'b'
	CALL	PRINT
	RETURN
PRINT_c
	MOVLW	'c'
	CALL	PRINT
	RETURN
PRINT_d
	MOVLW	'd'
	CALL	PRINT
	RETURN
PRINT_e
	MOVLW	'e'
	CALL	PRINT
	RETURN
PRINT_f
	MOVLW	'f'
	CALL	PRINT
	RETURN
PRINT_g
	MOVLW	'g'
	CALL	PRINT
	RETURN
PRINT_h
	MOVLW	'h'
	CALL	PRINT
	RETURN
PRINT_i
	MOVLW	'i'
	CALL	PRINT
	RETURN
PRINT_j
	MOVLW	'j'
	CALL	PRINT
	RETURN
PRINT_k
	MOVLW	'k'
	CALL	PRINT
	RETURN
PRINT_l
	MOVLW	'l'
	CALL	PRINT
	RETURN
PRINT_m
	MOVLW	'm'
	CALL	PRINT
	RETURN
PRINT_n
	MOVLW	'n'
	CALL	PRINT
	RETURN
PRINT_ñ
	MOVLW	'ñ'
	CALL	PRINT
	RETURN
PRINT_o
	MOVLW	'o'
	CALL	PRINT
	RETURN
PRINT_p
	MOVLW	'p'
	CALL	PRINT
	RETURN
PRINT_q
	MOVLW	'q'
	CALL	PRINT
	RETURN
PRINT_r
	MOVLW	'r'
	CALL	PRINT
	RETURN
PRINT_s
	MOVLW	's'
	CALL	PRINT
	RETURN
PRINT_t
	MOVLW	't'
	CALL	PRINT
	RETURN
PRINT_u
	MOVLW	'u'
	CALL	PRINT
	RETURN
PRINT_v
	MOVLW	'v'
	CALL	PRINT
	RETURN
PRINT_w
	MOVLW	'w'
	CALL	PRINT
	RETURN
PRINT_x
	MOVLW	'x'
	CALL	PRINT
	RETURN
PRINT_y
	MOVLW	'y'
	CALL	PRINT
	RETURN
PRINT_z
	MOVLW	'z'
	CALL	PRINT
	RETURN
PRINT_
	MOVLW	' '
	CALL	PRINT
	RETURN
PRINT_1
	MOVLW	'1'
	CALL	PRINT
	RETURN
PRINT_2
	MOVLW	'2'
	CALL	PRINT
	RETURN
PRINT_3
	MOVLW	'3'
	CALL	PRINT
	RETURN
PRINT_4
	MOVLW	'4'
	CALL	PRINT
	RETURN
PRINT_5
	MOVLW	'5'
	CALL	PRINT
	RETURN
PRINT_6
	MOVLW	'6'
	CALL	PRINT
	RETURN
PRINT_7
	MOVLW	'7'
	CALL	PRINT
	RETURN
PRINT_8
	MOVLW	'8'
	CALL	PRINT
	RETURN
PRINT_9
	MOVLW	'9'
	CALL	PRINT
	RETURN
PRINT_0
	MOVLW	'0'
	CALL	PRINT
	RETURN

;====================================================DATA_SEND==================================================
;Rutina que envia los datos a travez del puero serial (SPI. Primero envia un 0 al E de la pantalla, carga el dato
;en que se quiere enviar "VALB". Luego manda el dato de "VALA" (E=0/1 y RS=0/1) y finalmente vulve a enviar un 
;0 al E de la pantalla.
;==============================================================================================================
;===========================================Deshabilita LCD E=0================================================
INSTRUCCION
	BANKSEL	PORTA				;Cambia de banco
	BCF		LCD					;Deshabilita el LCD
	BANKSEL	SSP1BUF				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	b'01000000'			;Carga el OPCODE del MCP23S17 (Direccion =000)
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	0X12				;Carga la direccion del puero A del MCP23S17
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	B'00111111'			;RS=0 Y E=0
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PORTA				;Cambia de banco
	BSF		LCD					;Habilita el LCD
;=====================================IMPRIME DATO B============================================
	BANKSEL	PORTA				;Cambia de banco
	BCF		LCD					;Deshabilita el LCD
	BANKSEL	SSP1BUF				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	b'01000000'			;Carga el OPCODE del MCP23S17 (Direccion =000)
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	0X13				;Carga la direccion del puero B del MCP23S17
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	BANKSEL	VALB				;Cambia de banco
	MOVF	VALB,W				;Carga el contenido de VALB a W para ser enviado por el SPI
	BANKSEL	PIR1				;Cambia de banco
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PORTA				;Cambia de banco
	BSF		LCD					;Habilita el LCD
;=========================================IMPRIME DATO A (RS Y E)===============================================
	BANKSEL	PORTA				;Cambia de banco
	BCF		LCD					;Deshabilita el LCD
	BANKSEL	SSP1BUF				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	b'01000000'			;Carga el OPCODE del MCP23S17 (Direccion =000)
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	0X12				;Carga la direccion del puero A del MCP23S17
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	BANKSEL	VALB				;Cambia de banco
	MOVF	VALA,W				;Carga el contenido de VALA a W para ser enviado por el SPI
	BANKSEL	PIR1				;Cambia de banco
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PORTA				;Cambia de banco
	BSF		LCD					;Habilita el LCD
;===========================================Deshabilita LCD E=0================================================
	BANKSEL	PORTA				;Cambia de banco
	BCF		LCD					;Deshabilita el LCD
	BANKSEL	SSP1BUF				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	b'01000000'			;Carga el OPCODE del MCP23S17 (Direccion =000)
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	0X12				;Carga la direccion del puero A del MCP23S17
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PIR1				;Cambia de banco
	BCF		PIR1,SSP1IF			;Limpia el Flag de transmicion de datos
	MOVLW	B'00111111'			;RS=0 Y E=0
	MOVWF	SSP1BUF				;Envia el dato que esta en W al Bufer del SPI
	CALL	CHEK				;Llama a la rutina CHEK que verifica si ya se envio el dato
	BANKSEL	PORTA				;Cambia de banco
	BSF		LCD					;Habilita el LCD
	RETURN						;Retorna a la siguiente linea del CALL que llamo a la rutina
;===================================================CHEQUEO_SPI==============================================
CHEK
	BTFSS	PIR1,SSP1IF			;Revisa si el Flag de transmicion de datos esta encendido
	GOTO	$-2					;No, vuelve a preguntar
	BCF		PIR1,SSP1IF			;Si, Limpia el Flag de transmicion de datos
	MOVF	SSP1BUF,w			;Mueve a W lo que esta en bufer del SPI (SSP1BUF)
	RETURN						;Retorna a la siguiente linea del CALL que llamo a la rutina