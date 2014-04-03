

!!==============================================================================
!!
!!	AIM AND FIRE
!!	Sistema de apuntado y disparo QTE (Quick Time Event)
!!
!!==============================================================================
!!
!!	File:			aimAndFire.h
!!	Author(s):		J. Francisco Martín <jfm.lisaso@gmail.com>
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Máquina-Z / GLULX
!!	Version:		4.0
!!	Released:		2014/03/19
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	4.0: 2014/03/19	El objeto controlador ahora permite al usuario especificar 
!!					diferentes parámetros, como la ventana en que imprimir la 
!!					animación de apuntado y el tipo de movimiento de la 
!!					retícula, por ejemplo.
!!	3.1: 2014/02/06	Modo de depuración.
!!	3.0: 2012/07/05	Versión preliminar de la rutina updateGridPosition_SHM() 
!!					para mover la retícula con las ecuaciones del Movimiento 
!!					Armónico Simple (Simple Harmonic Motion). Ver NOTA #1.
!!	2.0: 2012		Refactorización del código, correcciones a la lógica y 
!!					adición de comentarios.
!!	1.0: 2012		Versión original de la librería.
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2012, 2014, J. Francisco Martín
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los términos de la Licencia Pública General GNU 
!!	publicada por la Fundación para el Software Libre, ya sea la versión 
!!	3 de la Licencia, o (a su elección) cualquier versión posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea útil, pero 
!!	SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita MERCANTIL o 
!!	de APTITUD PARA UN PROPÓSITO DETERMINADO. Consulte los detalles de 
!!	la Licencia Pública General GNU para más información.
!!
!!	Debería haber recibido una copia de la Licencia Pública General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
!!
!!	# INSTRUCCIONES DE INSTALACIÓN
!!
!!	Para utilizar las funciones de tiempo real de la librería se debe incluir 
!!	el siguiente punto de entrada glk en el código del relato interactivo (las 
!!	funciones de tiempo real funcionan únicamente en GLULX):
!!
!!		[ HandleGlkEvent ev; 
!!			if (ev-->0 == 1) ! evtype_Timer
!!				 AimingManager.run();
!!		];
!!
!!------------------------------------------------------------------------------
System_file;

!! Descomentar para obtener información de depuración:
!Constant DEBUG_AIMANDFIRE;

!! Tipos de movimiento de la animación de apuntado:
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
		!! posición final de la retícula después de que el jugador haya 
		!! detenido la animación:
		!!
		!!	@return integer
		!!		milisegundos de espera tras detener la animación
		!!----------------------------------------------------------------------
		get_delay_value [;
			return self.delay_value;
		],
		!!----------------------------------------------------------------------
		!! Retorna el tipo de movimiento de la animación establecido.
		!!
		!!	@return integer
		!!		código del tipo de movimiento
		!!----------------------------------------------------------------------
		get_motion_type [;
			return self.motion_type;
		],
		!!----------------------------------------------------------------------
		!! Retorna la ventana de estado sobre la que se dibuja la animación de 
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
		!! Operación principal del controlador. Invocada en cada ciclo del 
		!! temporizador glk. Imprime la animación de la retícula de apuntado y 
		!! su posición final una vez detenida por el jugador.
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
			!! Mientras la animación no sea detenida, se imprime:
			if (~~self.end_flag) {
				self.update_grid_position(self.motion_type);
				self.draw_aiming_line();
			}
			!! Tras ser detenida, la posición final de la retícula aún se 
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
		!! Establece los milisegundos de espera tras detener la animación.
		!!
		!!	@param delay:integer
		!!		milisegundos de espera tras detener la animación
		!!----------------------------------------------------------------------
		set_delay_value [ delay;
			self.delay_value = delay;
		],
		!!----------------------------------------------------------------------
		!! Establece el tipo de movimiento de la animación de apuntado.
		!!
		!!	@param type:integer
		!!		código del tipo de movimiento
		!!----------------------------------------------------------------------
		set_motion_type [ type;
			self.motion_type = type;
		],
		!!----------------------------------------------------------------------
		!! Establece la ventana de estado sobre la que se dibuja la animación 
		!! de apuntado.
		!!
		!!	@param status_win
		!!		ventana de estado sobre la que dibujar la animación de apuntado
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
		!! Inicia una operación de apuntado. Se encarga de inicializar los 
		!! atributos del sistema y lanzar los eventos del temporizador glk. 
		!! Puede invocarse con un parámetro opcional *flag*, que permite 
		!! establecer el punto de inicio de la retícula en la animación de 
		!! apuntado; si es falso la retícula comienza a moverse hacia la 
		!! derecha desde el extremo izquierdo de la ventana de estado, en caso 
		!! contrario, la retícula empieza desde el lugar y en el sentido que 
		!! tenía al detener la animación la última vez que se invocó el sistema.
		!!
		!!	@param flag:boolean (opcional)
		!!		establece el punto de inicio y sentido del movimiento de la 
		!!		retícula en la animación de apuntado
		!!----------------------------------------------------------------------
		start [ flag;
			!! Establece el punto de inicio y sentido del movimiento de la ret.:
			if (~~flag) {
				self.grid_direction = 0;
				self.grid_position = 0;
			}
			!! Se asegura que los otros atributos del gestor estén 
			!! correctamente inicializados:
			self.counter = 0;
			self.end_flag = false;
			!! Inicia el temporizador glk:
			glk(214, self.timer_frequency); ! glk_request_timer_events(t_freq);
		],
		!!----------------------------------------------------------------------
		!! Detiene la animación de apuntado y retorna el resultado.
		!!
		!!	@return result:int
		!!		distancia entre el centro de la ventana de estado y el punto en 
		!!		el que se detiene la retícula
		!!----------------------------------------------------------------------
		stop [ result;
			self.end_flag = true;
			result = self.grid_position + 1 - (self.status_window_width / 2);
			if (result < 0) result = -result;
			#Ifdef DEBUG_AIMANDFIRE;
			print "** Posición de la retícula: ";
			print self.grid_position, " / ", self.status_window_width, " **^";
			print "** Resultado: ", result, " **^";
			#Endif; ! DEBUG_AIMANDFIRE;
			
			return result;
		],
 private
		!!----------------------------------------------------------------------
		!! Dibuja la animación de apuntado.
		!!----------------------------------------------------------------------
		draw_aiming_line [;
			!! Si no está definida no se intenta dibujar en la ventana de estado:
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
			!! Se imprimen las guías:
			! window_move_cursor
			glk($002B, self.status_window, (self.status_window_width/2)-3, 0);
			print "·";
			! window_move_cursor
			glk($002B, self.status_window, (self.status_window_width/2)+3, 0);
			print "·";
			!! Se imprime la retícula:
			! window_move_cursor
			glk($002B, self.status_window, self.grid_position, 0);
			print (string) self.grid;
			! set_window
			glk($002F, gg_mainwin);
		],
		!!----------------------------------------------------------------------
		!! Actualiza la posición de la retícula.
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
		!! Actualiza la pos. de la retícula (Movimiento Rectilíneo Uniforme).
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
		!! Actualiza la pos. de la retícula (Movimiento Armónico Simple). 
		!! XXX - NOTA #1: NO USAR. La lógica de la rutina está programada a 
		!! modo de prueba y no produce una animación adecuada. Además utiliza 
		!! operaciones de coma flotante que requieren un compilador GLULX 
		!! que las soporte (por esta razón se ha decidido dejar el código 
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
		!! Cantidad de milisegundos que se sigue mostrando la posición final de 
		!! la retícula después de haber sido detenida la animación:
		delay_value 100,
		!! Indica que el jugador ha detenido la animación de la retícula:
		end_flag false,
		!! Retícula de apuntado:
		grid "[+]",
		!! Sentido del movimiento de la retícula:
		grid_direction 0,
		!! Longitud en caracteres de la retícula:
		grid_length 3,
		!! Posición de la retícula:
		grid_position 0,
		!! Código del tipo de movimiento de la animación de apuntado:
		motion_type 0,
		!! Ventana de estado sobre la que se dibuja la animación de apuntado:
		status_window 0,
		!! Guarda el ancho de la ventana de estado:
		status_window_width 0,
		!! Frecuencia con que se activa el temporizador glk:
		timer_frequency 10,
;
#Endif; ! TARGET_GLULX;

!!==============================================================================
!!	Rutina encargada de lanzar el sistema de apuntado y disparo. Puede 
!!	invocarse con el siguiente parámetro opcional:
!!
!!	 *	flag: (true/false) si se invoca con el parámetro activado, la animación 
!!		comenzará desde el extremo izquierdo de la ventana de estado, y 
!!		moviéndose hacia la derecha. En caso contrario, la animación comenzará 
!!		desde el punto y con el sentido que tenía en el último momento en que 
!!		fue invocada.
!!
!!	Retorna la distancia entre la retícula, una vez se haya detenido, y el 
!!	centro de la ventana de estado, -2 si se invoca desde Máquina-Z, y -1 en 
!!	caso de producirse algún error.
!!------------------------------------------------------------------------------

[ AimAndFire flag		result;
	#Ifdef TARGET_ZCODE;

	#Ifdef DEBUG_AIMANDFIRE;
	print "** AimAndFire: apuntado y disparo sobre Máquina-Z. **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	!! Establece el resultado de la operación en Máquina-Z:
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
	!! La animación de apuntado se detiene cuando el jugador pulsa una tecla:
	KeyCharPrimitive();
	result = AimingManager.stop();

	#Endif; ! TARGET_

	return result;
];


