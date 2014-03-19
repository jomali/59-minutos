

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
!!	Version:		4.0
!!	Released:		2014/03/19
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	4.0: 2014/03/19	El objeto controlador ahora permite al usuario especificar 
!!					diferentes par�metros, como la ventana en que imprimir la 
!!					animaci�n de apuntado y el tipo de movimiento de la 
!!					ret�cula, por ejemplo.
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
Message "Incluyendo sistema de apuntado y disparo [aimAndFire 4.0]";

!! Descomentar para obtener informaci�n de depuraci�n:
!Constant DEBUG_AIMANDFIRE;

!! Tipos de movimiento de la animaci�n de apuntado:
Constant LINEAR_MOTION 1;
Constant SIMPLE_HARMONIC_MOTION 2;


!!==============================================================================
!!	Objeto gestor del sistema de apuntado. Implementa la siguiente interfaz:
!!	 *	get_delay_value(): return integer
!!	 *	get_motion_type(): return integer
!!	 *	get_status_window(): return status_window
!!	 *	get_timer_frequency(): return integer
!!	 *	run()
!!	 *	set_delay_value(delay:integer)
!!	 *	set_motion_type(motion_type:integer)
!!	 *	set_status_window(status_win)
!!	 *	set_timer_frequency(freq:integer)
!!	 *	start(flag:boolean)
!!	 *	stop(): return integer
!!------------------------------------------------------------------------------

#Ifdef TARGET_GLULX;
Object	AimingManager "(Aiming Manager)"
 with	!!----------------------------------------------------------------------
		!! Retorna la cantidad de milisegundos que se sigue mostrando la 
		!! posici�n final de la ret�cula despu�s de que el jugador haya 
		!! detenido la animaci�n:
		!!
		!!	@return integer
		!!		milisegundos de espera tras detener la animaci�n
		!!----------------------------------------------------------------------
		get_delay_value [;
			return self.delay_value;
		],
		!!----------------------------------------------------------------------
		!! Retorna el tipo de movimiento de la animaci�n establecido.
		!!
		!!	@return integer
		!!		c�digo del tipo de movimiento
		!!----------------------------------------------------------------------
		get_motion_type [;
			return self.motion_type;
		],
		!!----------------------------------------------------------------------
		!! Retorna la ventana de estado sobre la que se dibuja la animaci�n de 
		!! apuntado.
		!!
		!!	@return status_window
		!!----------------------------------------------------------------------
		get_status_window [;
			return self.status_window;
		],
		!!----------------------------------------------------------------------
		!! Retorna la frecuencia con la que el gestor activa el temporizador 
		!! glk (valor en milisegundos).
		!!
		!!	@return integer
		!!		frecuencia (en milisegundos)
		!!----------------------------------------------------------------------
		get_timer_frequency [;
			return self.timer_frequency;
		],
		!!----------------------------------------------------------------------
		!! Operaci�n principal del controlador. Invocada en cada ciclo del 
		!! temporizador glk. Imprime la animaci�n de la ret�cula de apuntado y 
		!! su posici�n final una vez detenida por el jugador.
		!!----------------------------------------------------------------------
		run [;
			if (self.status_window == 0) {
				glk(214, 0); ! glk_request_timer_events(0);
				return false;
			}
			!! Se calcula el ancho de la ventana de estado (debe calcularse a 
			!! cada evento del temporizador por si la ventana ha sido 
			!! redimensionada por el jugador):
			glk($0025, self.status_window, gg_arguments, gg_arguments + 4);
			self.status_window_width = gg_arguments-->0;
			!! Mientras la animaci�n no sea detenida, se imprime:
			if (~~self.end_flag) {
				self.update_grid_position(self.motion_type);
				self.draw_aiming_line();
			}
			!! Tras ser detenida, la posici�n final de la ret�cula a�n se 
			!! muestra en la ventana de estado durante unos instantes:
			else {
				self.draw_aiming_line();
				self.counter++;
				if (self.counter > self.delay_value) {
					glk(214, 0); ! glk_request_timer_events(0);
					self.counter = 0;
					self.end_flag = false;
					DrawStatusLine();
				}
			}
		],
		!!----------------------------------------------------------------------
		!! Establece los milisegundos de espera tras detener la animaci�n.
		!!
		!!	@param delay:integer
		!!		milisegundos de espera tras detener la animaci�n
		!!----------------------------------------------------------------------
		set_delay_value [ delay;
			self.delay_value = delay;
		],
		!!----------------------------------------------------------------------
		!! Establece el tipo de movimiento de la animaci�n de apuntado.
		!!
		!!	@param type:integer
		!!		c�digo del tipo de movimiento
		!!----------------------------------------------------------------------
		set_motion_type [ type;
			self.motion_type = type;
		],
		!!----------------------------------------------------------------------
		!! Establece la ventana de estado sobre la que se dibuja la animaci�n 
		!! de apuntado.
		!!
		!!	@param status_win
		!!		ventana de estado sobre la que dibujar la animaci�n de apuntado
		!!----------------------------------------------------------------------
		set_status_window [ status_win;
			self.status_window = status_win;
		],
		!!----------------------------------------------------------------------
		!! Establece la frecuencia con la que el gestor activa el temporizador 
		!! glk (valor en milisegundos).
		!!
		!!	@param freq:integer
		!!		frecuencia (en milisegundos)
		!!----------------------------------------------------------------------
		set_timer_frequency [ freq;
			self.timer_frequency = freq;
		],
		!!----------------------------------------------------------------------
		!! Inicia una operaci�n de apuntado. Se encarga de inicializar los 
		!! atributos del sistema y lanzar los eventos del temporizador glk. 
		!! Puede invocarse con un par�metro opcional *flag*, que permite 
		!! establecer el punto de inicio de la ret�cula en la animaci�n de 
		!! apuntado; si es falso la ret�cula comienza a moverse hacia la 
		!! derecha desde el extremo izquierdo de la ventana de estado, en caso 
		!! contrario, la ret�cula empieza desde el lugar y en el sentido que 
		!! ten�a al detener la animaci�n la �ltima vez que se invoc� el sistema.
		!!
		!!	@param flag:boolean (opcional)
		!!		establece el punto de inicio y sentido del movimiento de la 
		!!		ret�cula en la animaci�n de apuntado
		!!----------------------------------------------------------------------
		start [ flag;
			!! Establece el punto de inicio y sentido del movimiento de la ret.:
			if (~~flag) {
				self.grid_direction = 0;
				self.grid_position = 0;
			}
			!! Se asegura que los otros atributos del gestor est�n 
			!! correctamente inicializados:
			self.counter = 0;
			self.end_flag = false;
			!! Inicia el temporizador glk:
			glk(214, self.timer_frequency); ! glk_request_timer_events(t_freq);
		],
		!!----------------------------------------------------------------------
		!! Detiene la animaci�n de apuntado y retorna el resultado.
		!!
		!!	@return result:int
		!!		distancia entre el centro de la ventana de estado y el punto en 
		!!		el que se detiene la ret�cula
		!!----------------------------------------------------------------------
		stop [ result;
			self.end_flag = true;
			result = self.grid_position + 1 - (self.status_window_width / 2);
			if (result < 0) result = -result;
			#Ifdef DEBUG_AIMANDFIRE;
			print "** Posici�n de la ret�cula: ";
			print self.grid_position, " / ", self.status_window_width, " **^";
			print "** Resultado: ", result, " **^";
			#Endif; ! DEBUG_AIMANDFIRE;
			
			return result;
		],
 private
		!!----------------------------------------------------------------------
		!! Dibuja la animaci�n de apuntado.
		!!----------------------------------------------------------------------
		draw_aiming_line [;
			!! Si no est� definida no se intenta dibujar en la ventana de estado:
			if (self.status_window == 0) {
				#Ifdef DEBUG_AIMANDFIRE;
				print "** AimingManager: No hay ventana de estado. **^";
				#Endif; ! DEBUG_AIMANDFIRE;
				return;
			}
			! set_window
			glk($002F, self.status_window);
			StatusLineHeight(gg_statuswin_size); !! TODO
			! window_clear
			glk($002A, self.status_window);
			!! Se imprimen las gu�as:
			! window_move_cursor
			glk($002B, self.status_window, (self.status_window_width/2)-3, 0);
			print "�";
			! window_move_cursor
			glk($002B, self.status_window, (self.status_window_width/2)+3, 0);
			print "�";
			!! Se imprime la ret�cula:
			! window_move_cursor
			glk($002B, self.status_window, self.grid_position, 0);
			print (string) self.grid;
			! set_window
			glk($002F, gg_mainwin);
		],
		!!----------------------------------------------------------------------
		!! Actualiza la posici�n de la ret�cula.
		!!----------------------------------------------------------------------
		update_grid_position [ motion_type;
			switch (motion_type) {
				SIMPLE_HARMONIC_MOTION:
					self.update_grid_position_SHM();
				default:
					self.update_grid_position_LM();
			}
		],
		!!----------------------------------------------------------------------
		!! Actualiza la pos. de la ret�cula (Movimiento Rectil�neo Uniforme).
		!!----------------------------------------------------------------------
		update_grid_position_LM [; ! Linear Motion
			!! Sentido del movimiento: hacia la derecha
			if (self.grid_direction == 0) {
				if (self.grid_position < self.status_window_width 
					- (self.grid_length + 1)) {
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
		!! Actualiza la pos. de la ret�cula (Movimiento Arm�nico Simple). 
		!! XXX - NOTA #1: NO USAR. La l�gica de la rutina est� programada a 
		!! modo de prueba y no produce una animaci�n adecuada. Adem�s utiliza 
		!! operaciones de coma flotante que requieren un compilador GLULX 
		!! que las soporte (por esta raz�n se ha decidido dejar el c�digo 
		!! comentado).
		!!----------------------------------------------------------------------
		update_grid_position_SHM [ i ini sini aux gp; ! Simple Harmonic Motion
			i = ini + sini + aux + gp;
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
		],
		!!----------------------------------------------------------------------
		!! Atributos:
		!!----------------------------------------------------------------------
		!! Contador:
		counter 0,
		!! Cantidad de milisegundos que se sigue mostrando la posici�n final de 
		!! la ret�cula despu�s de haber sido detenida la animaci�n:
		delay_value 100,
		!! Indica que el jugador ha detenido la animaci�n de la ret�cula:
		end_flag false,
		!! Ret�cula de apuntado:
		grid "[+]",
		!! Sentido del movimiento de la ret�cula:
		grid_direction 0,
		!! Longitud en caracteres de la ret�cula:
		grid_length 3,
		!! Posici�n de la ret�cula:
		grid_position 0,
		!! C�digo del tipo de movimiento de la animaci�n de apuntado:
		motion_type 0,
		!! Ventana de estado sobre la que se dibuja la animaci�n de apuntado:
		status_window 0,
		!! Guarda el ancho de la ventana de estado:
		status_window_width 0,
		!! Frecuencia con que se activa el temporizador glk:
		timer_frequency 10,
;
#Endif; ! TARGET_GLULX;

!!==============================================================================
!!	Rutina encargada de lanzar el sistema de apuntado y disparo. Puede 
!!	invocarse con el siguiente par�metro opcional:
!!
!!	 *	flag: (true/false) si se invoca con el par�metro activado, la animaci�n 
!!		comenzar� desde el extremo izquierdo de la ventana de estado, y 
!!		movi�ndose hacia la derecha. En caso contrario, la animaci�n comenzar� 
!!		desde el punto y con el sentido que ten�a en el �ltimo momento en que 
!!		fue invocada.
!!
!!	Retorna la distancia entre la ret�cula, una vez se haya detenido, y el 
!!	centro de la ventana de estado, -2 si se invoca desde M�quina-Z, y -1 en 
!!	caso de producirse alg�n error.
!!------------------------------------------------------------------------------

[ AimAndFire flag		result;
	#Ifdef TARGET_ZCODE;

	#Ifdef DEBUG_AIMANDFIRE;
	print "** AimAndFire: apuntado y disparo sobre M�quina-Z. **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	!! Establece el resultado de la operaci�n en M�quina-Z:
	result = -2;

	#Ifnot; ! TARGET_GLULX;

	#Ifdef DEBUG_AIMANDFIRE;
	print "** AimAndFire: apuntado y disparo sobre Glulx. **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	!! Comprobaciones previas sobre el gestor del sistema de apuntado y disparo:
	if (AimingManager.get_motion_type() == 0)
		AimingManager.set_motion_type(LINEAR_MOTION);
	if (AimingManager.get_status_window() == 0)
		AimingManager.set_status_window(gg_statuswin);
	!! Se inicia el sistema de apuntado y disparo:
	AimingManager.start(flag);
	!! La animaci�n de apuntado se detiene cuando el jugador pulsa una tecla:
	KeyCharPrimitive();
	result = AimingManager.stop();

	#Endif; ! TARGET_

	return result;
];


