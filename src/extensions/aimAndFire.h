

!!==============================================================================
!!
!!	AIM AND FIRE
!!	Sistema de apuntado y disparo QTE (Quick Time Event)
!!
!!==============================================================================
!!
!!	File:			aimAndFire.h
!!	Author(s):		J. Francisco Mart�n <jfm.lisaso@gmail.com>
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		M�quina-Z / GLULX
!!	Version:		3.1
!!	Released:		2014/02/06
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	3.1: 2014/02/06	Modo de depuraci�n.
!!	3.0: 2012/07/05	Versi�n preliminar de la rutina updateGridPosition_SHM() 
!!					para mover la ret�cula con las ecuaciones del Movimiento 
!!					Arm�nico Simple (Simple Harmonic Motion). Ver NOTA #1.
!!	2.0: 2012		Refactorizaci�n del c�digo, correcciones a la l�gica y 
!!					adici�n de comentarios.
!!	1.0: 2012		Versi�n original de la librer�a.
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2012, 2014, J. Francisco Mart�n
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los t�rminos de la Licencia P�blica General GNU 
!!	publicada por la Fundaci�n para el Software Libre, ya sea la versi�n 
!!	3 de la Licencia, o (a su elecci�n) cualquier versi�n posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea �til, pero 
!!	SIN GARANT�A ALGUNA; ni siquiera la garant�a impl�cita MERCANTIL o 
!!	de APTITUD PARA UN PROP�SITO DETERMINADO. Consulte los detalles de 
!!	la Licencia P�blica General GNU para m�s informaci�n.
!!
!!	Deber�a haber recibido una copia de la Licencia P�blica General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
!!
!!	# INSTRUCCIONES DE INSTALACI�N
!!
!!	Para utilizar las funciones de tiempo real de la librer�a se debe incluir 
!!	el siguiente punto de entrada glk en el c�digo del relato interactivo (las 
!!	funciones de tiempo real funcionan �nicamente en GLULX):
!!
!!		[ HandleGlkEvent ev; 
!!			if (ev-->0 == 1) ! evtype_Timer
!!				 AimingManager.run();
!!		];
!!
!!------------------------------------------------------------------------------
System_file;

!! Descomentar para obtener informaci�n de depuraci�n:
!Constant DEBUG_AIMANDFIRE;


!!==============================================================================
!!	Objeto gestor del sistema de apuntado.
!!------------------------------------------------------------------------------

#Ifdef	TARGET_GLULX;
Object	AimingManager
 with	!!----------------------------------------------------------------------
		!! Atributos:
		!!----------------------------------------------------------------------
		timer_frequency	10,		! Freq. de los eventos del timer glk
		delay			100,	! Retraso tras detener la animaci�n
		counter			0,		! Contador
		end_flag		false,	! Indica el final de la operaci�n de apuntado
		width			0,		! Ancho de la l�nea de apuntado
		grid			"[+]",	! Ret�cula de apuntado
		grid_length		3,		! Longitud (en caracteres) de la ret�cula
		grid_position	0,		! Guarda la posici�n de la ret�cula
		grid_direction	0,		! Guarda el sentido de movimiento de la ret�cula
		activado		false,
		!!----------------------------------------------------------------------
		!! Operaci�n principal del controlador. Invocada en cada ciclo del 
		!! reloj glk. Imprime la animaci�n de la ret�cula de apuntado y su 
		!! posici�n final una vez detenida.
		!!----------------------------------------------------------------------
		run [;
			if (~~self.activado) return false;
			!! Obtiene el ancho de la ventana de estado:
			glk($0025, statusBar.winid, gg_arguments, gg_arguments+4);
			self.width = gg_arguments-->0;
			!! Apuntado finalizado. Muestra el resultado durante unos instantes:
			if (self.end_flag) self.finalDelay(); 
			!! Calcula posici�n de ret�cula e imprime la animaci�n:
			else {
				self.updateGridPosition_LM(); ! Linear Motion
				self.drawAimingLine();
			}
		],
		!!----------------------------------------------------------------------
		!! Inicializa los atributos y lanza los eventos de reloj glk. Se se 
		!! invoca sin el par�metro flag (con con flag==false), la animaci�n de 
		!! de la ret�cula comenzar� desde el punto y en el sentido en que fue 
		!! detenida la �ltima vez. En caso contrario, la animaci�n comenzar� 
		!! desde el extremo izquierdo de la ventana de estado y movi�ndose 
		!! hacia la derecha.
		!!	@param flag
		!!----------------------------------------------------------------------
		initialize [ flag;
			if (flag) {
				self.grid_position  = 0;
				self.grid_direction = 0;
			}
			self.counter = 0;
			self.end_flag = false;
			self.activado = true;
			!! Lanza los eventos de reloj glk
			glk(214, self.timer_frequency); ! glk_request_timer_events(t_freq);
		],
		!!----------------------------------------------------------------------
		!! Retorna la posici�n de la ret�cula.
		!!	@return int grid_position
		!!----------------------------------------------------------------------
		getGridPosition [; return self.grid_position; ],
		!!----------------------------------------------------------------------
		!! Retorna el ancho de la barra de apuntado.
		!!	@return int width
		!!----------------------------------------------------------------------
		getAimingLineWidth [; return self.width; ],
		!!----------------------------------------------------------------------
		!! Indica el final de la operaci�n de apuntado.
		!!----------------------------------------------------------------------
		aimingEnded [; self.end_flag = true; ],
		!!----------------------------------------------------------------------
		!! Calcula la posici�n de la ret�cula (Movimiento Rectil�neo Uniforme)
		!!----------------------------------------------------------------------
		updateGridPosition_LM [; ! Linear Motion
			!! Sentido del movimiento: hacia la derecha
			if (self.grid_direction == 0) {
				if (self.grid_position < self.width - (self.grid_length+1)) {
					self.grid_position++;
				}
				else {
					self.grid_direction = 1;
					self.grid_position--;
				}
			}
			!! Sentido del movimiento: hacia la izquierda
			else {
				if (self.grid_position > 1) {
					self.grid_position--;
				}
				else {
					self.grid_direction = 0;
					self.grid_position++;
				}
			}
		],
		!!----------------------------------------------------------------------
		!! Calcula la posici�n de la ret�cula (Movimiento Arm�nico Simple). 
		!! XXX - NOTA #1: En el estado actual, la animaci�n no se imprime 
		!! correctamente, por eso la rutina est� comentada.
		!!----------------------------------------------------------------------
!		updateGridPosition_SHM [ i ini sini aux gp; ! Simple Harmonic Motion
!			aux = WIN_WIDTH/2;
!			@numtof aux ini; ! ini: centro de la ventana
!			@numtof GRID_COUNTER i; ! i: contador
!			@sin i sini; ! sini: seno de GRID_COUNTER
!			@fmul ini sini aux; ! aux: (WIN_WIDTH/2)*sin(GRID_COUNTER)
!
!			if (GRID_MOVEMENT == 0) @fadd ini aux gp;
!			else @fsub ini aux gp;
!			@ftonumn gp GRID_POS;
!
!			GRID_COUNTER++;
!			if (GRID_COUNTER == 180)
!			{	GRID_COUNTER = 0;
!				if (GRID_MOVEMENT == 0) GRID_MOVEMENT = 1;
!				else GRID_MOVEMENT = 0;
!			}
!		],
		!!----------------------------------------------------------------------
		!! Dibuja la animaci�n de apuntado
		!!----------------------------------------------------------------------
		drawAimingLine [;
			! If we have no status window, we must not try to redraw it.
			if (statusBar.winid == 0) {
				print "** AimingManager error. No hay ventana de estado. **^";
				return;
			}
			glk($002F, statusBar.winid); ! set_window
			StatusLineHeight(gg_statuswin_size);
			glk($002A, statusBar.winid); ! window_clear
			! Se imprimen las gu�as:
			glk($002B, statusBar.winid, (self.width/2)-3, 0); ! window_move_cursor
			print "�";
			glk($002B, statusBar.winid, (self.width/2)+3, 0); ! window_move_cursor
			print "�";
			! Se imprime la ret�cula:
			glk($002B, statusBar.winid, self.getGridPosition(), 0);
			print (string) self.grid;
			glk($002F, gg_mainwin); ! set_window
		],
		!!----------------------------------------------------------------------
		!! Una vez detenida la animaci�n de apuntado a�n se muestra la posici�n 
		!! final de la ret�cula durante unos momentos (tiempo establecido por 
		!! el atributo delay). Al cabo, se detienen los eventos del timer y se 
		!! imprime la barra de estado normal.
		!!----------------------------------------------------------------------
		finalDelay [;
			self.drawAimingLine();
			self.counter++;
			if (self.counter > self.delay) {
				glk(214, 0); ! glk_request_timer_events(0);
				self.activado = false;
				DrawStatusLine();
			}
		],
;
#Endif;	! TARGET_GLULX;


!!==============================================================================
!!	Lanza el sistema de apuntado y disparo:
!!	@return int: 
!!		-2, (respuesta del sistema en M�quina-Z), 
!!		-1, (fallo),
!!		result (distancia entre la ret�cula y el centro).
!!------------------------------------------------------------------------------
!! TODO: controlar errores relativos a distintos anchos de ventana

[ AimAndFire grid_pos width result;
	#Ifdef	TARGET_ZCODE;

	grid_pos = 0; width = 0; result = 0; ! Suprime advertencias del compilador
	#Ifdef DEBUG_AIMANDFIRE;
	print "** AimAndFire: Apuntado y disparo para m�quina-Z. **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	return -2;

	#Ifnot;	! TARGET_GLULX;

	AimingManager.initialize();
	KeyCharPrimitive(); ! Pulsaci�n de tecla. Fin de la animaci�n
	AimingManager.aimingEnded();
	grid_pos = AimingManager.getGridPosition() + 1;
	width	 = AimingManager.getAimingLineWidth();
	#Ifdef DEBUG_AIMANDFIRE;
	print "** Posici�n de la ret�cula: ", grid_pos, "/", width, " **^";
	print "** ", grid_pos, " - ", (width/2), ", = ", grid_pos-(width/2), " **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	result = grid_pos - (width/2);
	if (result < 0 ) return -result;
	else return result;

	#Endif;	! TARGET_
];


