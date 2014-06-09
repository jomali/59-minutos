

!!==============================================================================
!!
!!	GSTATUSX WINDOW
!!	Módulo de ventana de estado para GWindows
!!
!!==============================================================================
!!
!!	File:			gstatusX.h
!!	Author(s):		J. Francisco Martín <jfm.lisaso@gmail.com>
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Z-Machine / Glulx
!!	Version:		1.0
!!	Released:		2014/06/06
!!
!!	GStatusXWin crea una línea de estado en la que se imprime el nombre, 
!!	centrado en la ventana, de la localidad actual del personaje protagonista.
!!
!!	Hereda de TextGrid.
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	1.0: 2014/06/06
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2014, J. Francisco Martín
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

#Ifndef GW_GSTATUS_H;
System_file;
Constant GW_GSTATUS_H;

!!------------------------------------------------------------------------------
#Ifndef PRINT_LOCATION_NAME;
Constant PRINT_LOCATION_NAME;
!! Buffer auxiliar para calcular la longitud de un texto:
#Ifdef	VN_1630;	! compilador 6.30 o superior
Array _AlmacenDeTexto_ buffer 160; ! 150 y algo caracteres deberían bastar
#Ifnot;				! compiladores más viejos
Array	_AlmacenDeTexto_ -> 160 + WORDSIZE;
#Endif;	!VN_1630
!! La siguiente función sirve tanto para imprimir el nombre de la localidad 
!! como para redirigir su salida a *PrintToBuffer()* para contar caracteres:
[ PrintLocationName;
	if (location == thedark) print (name) location;
	else {
		FindVisibilityLevels();
		if (visibility_ceiling == location) print (name) location;
		else print (The) visibility_ceiling;
	}
];
#Endif; ! PRINT_LOCATION_NAME;
!!------------------------------------------------------------------------------

Class	GStatusXWin
 class	TextGrid,
 with	redraw [; self.update(); ],
		update [ pos;
			if (location == 0 || player == 0 || parent(player) == 0) return;
			glk_set_window(self.winid);
			pos = (self.width - 
				PrintToBuffer(_AlmacenDeTexto_, 160, PrintLocationName)) / 2;
			glk_window_clear(self.winid);
			glk_window_move_cursor(self.winid, pos, 0);
			PrintLocationName();
		],
		split 1,
		split_dir winmethod_Above,
		stylehints style_Normal stylehint_Reversecolor 1,
 has	abssplit;

#Endif; ! GW_GSTATUS_H;


