! Inform / INFSP 6


!!==============================================================================
!!
!!	GUNS
!!	Armas de fuego simples
!!
!!==============================================================================
!!
!!	File:			class_gun.h
!!	Author(s):		J. Francisco Mart�n (jfm.lisaso@gmail.com)
!!	Language:		ES (Castellano)
!!	System:			Inform/INFSP 6
!!	Platform:		M�quina-Z / Glulx
!!	Version:		1.0
!!	Released:		2010/04/05
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2010, J. Francisco Mart�n
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
!! Calibres de pistola ordenados por rango (mm. y pulgadas)
!!		- 2 mm (.079+ caliber) peque�o
!!		- 3 mm (.118+ caliber)
!!		- 4 mm (.157+ caliber)
!!		- 5 mm (.197+ caliber)
!!		- 6 mm (.236+ caliber) medio
!!		- 7 mm (.276+ caliber)
!!		- 8 mm (.315+ caliber)
!!		- 9 mm (.354+ caliber)
!!		- 10 mm (.394+ caliber) grande
!!		- 11 mm (.433+ caliber)
!!		- 12 mm (.472+ caliber)
!!		- 13 mm (.511+ caliber)
!! [Fuente: Wikipedia]
!!
!!------------------------------------------------------------------------------
System_file;

Class	Gun
 with	caliber 9,	!! Calibre medio normal de 9mm.
		ammo 0;		!! Munici�n

[ Firearm obj x;
	if ( obj == nothing ) objectloop( x in player ) if (x ofclass Gun) obj = x;
	if ( obj == nothing ) {
		print "Necesitas un arma de fuego para hacer eso.^";
		return false;
	}
	if ( obj ofclass Gun ) {
		if ( obj.ammo > 0 ) {
			second = obj;
			if ( noun == u_obj or d_obj ) {
				second.ammo--;
				print "Apuntas hacia ";
				if ( noun == u_obj ) print (name) u_obj;
				else print (name) d_obj;
				print " con ", (the) second ," y disparas: �PANG!^";
				return false;
			}
			return true;
		}
		print "No te quedan balas en ", (the) obj ,".^";
		return false;
	}
	print "S�lo puedes hacer eso con armas de fuego.^";
	return false;
];

!!------------------------------------------------------------------------------

Verb	'dispara'
	*								-> Shoot
	* noun							-> Shoot
	* noun 'con' noun				-> Shoot
	* noun 'a//' noun				-> Shoot reverse
	* noun 'al' noun				-> Shoot
	* 'a//' noun					-> Shoot
	* 'a//' noun 'con' noun			-> Shoot reverse
	* 'al' noun						-> Shoot
	* 'al' noun 'con' noun			-> Shoot reverse
	* 'hacia' noun					-> Shoot
	* 'hacia' noun 'con' noun		-> Shoot reverse
;

Verb	'apunta' 'amartilla' 'enca�ona'
	*								-> ShootHelp
	* noun							-> ShootHelp
	* 'a//' noun					-> ShootHelp
	* 'a//' noun 'con' noun			-> ShootHelp
	* 'al' noun						-> ShootHelp
	* 'al' noun 'con' noun			-> ShootHelp
	* noun 'con' noun				-> ShootHelp
	* noun 'a//' noun				-> ShootHelp
	* noun 'al' noun				-> ShootHelp
;

!!------------------------------------------------------------------------------

Default NO_SHOOT "Mejor no.";

[ ShootSub;
	if ( noun == nothing ) noun = u_obj;
	if ( Firearm( second ) == true ) print (string) NO_SHOOT;
	return true;
];

[ ShootHelpSub;
	print_ret "Para usar un arma de fuego, teclea simplemente 
	DISPARA (a) OBJETIVO (con ARMA).";
];


