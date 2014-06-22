

!!==============================================================================
!!
!!	GRAMÁTICA ESPAÑOLA EXTENDIDA
!!
!!==============================================================================
!!
!!	File:			SpanishEG.inf
!!	Author(s):		Emily Short <emshort@mindspring.com>
!!					J. Francisco Martín <jfm.lisaso@gmail.com>
!!					Mapache
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Máquina-Z / GLULX
!!	Version:		3.3
!!	Released:		2014/06/22
!!
!!------------------------------------------------------------------------------
!!
!!	# NOTAS
!!
!!	Basada en "ExpertGrammar.h", de Emily Short <emshort@mindspring.com>
!!	y en su traducción al castellano "GramaticaExperta.h", de Mapache.
!!
!!
!!	# DERECHOS
!!
!!	Esta librería puede tratarse como de dominio público. Puede incluirse 
!!	con o sin citar al autor original. Puede ser modificada a discreción 
!!	por el usuario. Se puede distribuir libremente. Pueden extraerse 
!!	fragmentos de la librería.
!!
!!
!!	# INSTALACIÓN
!!
!!	Incluye "SpanishEG.h" después de SpanishG en tu archivo de juego.
!!
!!	Se puede definir la variable GRAMMATICAL_INFLECTION en el archivo de juego, 
!!	que puede tomar los valores: 1 (PRESENTE 1ª PERSONA), 2 (PRESENTE 2ª 
!!	PERSONA), 3 (PRESENTE 3ª PERSONA), 4 (PASADO 1ª PERSONA), 5 (PASADO 2ª 
!!	PERSONA) o 6 (PASADO 3ª PERSONA) para modificar la conjugación gramatical 
!!	de los mensajes de la librería. Si no se define ninguna 
!!	GRAMMATICAL_INFLECTION, la librería interpreta que se usa el valor 2: los 
!!	mensajes de "SpanishEG.h" se imprimen en presente y segunda persona.
!!
!!
!!	# NUEVAS ACCIONES
!!
!!	 *	Dance
!!	 *	GoDown
!!	 *	GoUp
!!	 *	KnockOn
!!	 *	Masturbate
!!	 *	Shout
!!	 *	Sit
!!	 *	SleepWith
!!	 *	StandUp
!!	 *	Untie
!!	 *	Use
!!	 *	Write
!!	 *	Xyzzy
!!
!!------------------------------------------------------------------------------

Default	GRAMMATICAL_INFLECTION 2;

!!------------------------------------------------------------------------------

Verb	'machaca'						= 'ataca';
Verb	'silba' 'tararea' 'tatarea'		= 'canta';
Verb	'roba' 'agarra'					= 'coge';
Verb	'talla' 'esculpe'				= 'corta';
Verb	'emplaza' 'deposita' 'abandona'	= 'deja';
Verb	'berrea' 'aulla'				= 'grita';
Verb	'marcha'						= 'ir';
Verb	'incinera' 'tuesta'				= 'quema';
VerboIrregular "tostar" with imperativo 'tuesta';
Verb	'acaricia'						= 'toca';

!!------------------------------------------------------------------------------

Extend	'sopla'
	* 'sobre' / 'dentro' / 'a' noun	-> Blow
	* 'a' 'traves' 'de' noun		-> Blow
;

Extend	'prende' last
	* 'fuego' noun					-> Burn
	* noun 'fuego'					-> Burn
	* 'fuego' 'a//' noun			-> Burn
	* 'a//' noun 'fuego'			-> Burn
	* 'fuego' noun 'con' held		-> Burn
	* noun 'fuego' 'con' held		-> Burn
	* 'fuego' 'a//' noun 'con' held	-> Burn
	* 'a//' noun 'fuego' 'con' held	-> Burn
;

Verb	'baila'
	*								-> Dance
;

Verb	'muerde'
	* noun							-> Eat
	* edible						-> Eat
!	* animate						-> Attack
!	* 'a' animate					-> Attack
; VerboIrregular "morder" with imperativo 'muerde';

Verb	'desciende'
	*								-> GoDown
	* noun							-> Enter
	* 'por' noun					-> Enter
; VerboIrregular "descender" with imperativo 'desciende';

Verb	'asciende'
	*								-> GoUp
	* noun							-> Enter
	* 'por' noun					-> Enter
; VerboIrregular "ascender" with imperativo 'asciende';

Verb	'plancha' 'estira' 'desarruga'
	* noun							-> Iron
;

Extend	'salta'
	* 'en' noun						-> JumpOver
;

Verb	'llama'
	* door							-> KnockOn
	* 'a' door						-> KnockOn
	* animate						-> Tell
	* 'a' animate					-> Tell
;

Extend only 'golpea' first
	* door							-> KnockOn
	* 'a' door						-> KnockOn
;

Extend	'mira'
	* 'alrededor'					-> Look
	* 'a' 'tu' 'alrededor'			-> Look
;

Verb	'masturba'
	* '-me'/'-te'					-> Masturbate
;

Extend	'lee' first
	* noun							-> Read
;

Extend	'busca'
	* 'bajo' noun					-> LookUnder
	* 'debajo' 'de' noun			-> LookUnder
	* 'entre' noun					-> Search
;

Extend	'mira'
	* 'entre' noun					-> Search
;

Extend	'grita' last
	*								-> Shout
;

Extend    'sienta' replace
    * 								-> Sit
	* '-te' / '-me'					-> Sit
	* noun							-> Sit
	* '-te' / '-me' noun			-> Sit
	* 'en' noun						-> Sit
	* '-te' / '-me'	'en' noun		-> Sit
; VerboIrregular "sentar" with imperativo 'sientate';

Extend	'duerme'
	* '-te'/'-me'					-> Sleep
	* noun							-> Sleep
	* '-te'/'-me' noun				-> Sleep
	* 'en' noun						-> Sleep
	* '-te'/'-me' 'en' noun			-> Sleep
;

Verb	'folla' 'copula'
	* animate						-> SleepWith
	* '-te' / '-me' animate			-> SleepWith
	* 'a//' / 'con' animate			-> SleepWith
	* '-te' / '-me' 'a' animate		-> SleepWith
;

Extend	only 'jode' first
	* animate						-> SleepWith
	* 'a//' / 'con' animate			-> SleepWith
;

Verb	'ten'
	* 'sexo' 'con' animate			-> SleepWith
	* 'coito' 'con' animate			-> SleepWith
;

Verb	'haz'
	* 'el' 'amor' 'con' animate		-> SleepWith
;

Extend	only 'levantate' 'levantarse' 'levantarte' first
	*								-> StandUp
	* 'de' noun						-> StandUp;
Verb    'levanta' 'incorpora'
    * 								-> StandUp
	* '-te'/'-me'					-> StandUp
    * 'de' noun 					-> StandUp
	* '-te'/'-me' 'de//' noun		-> StandUp
;

Verb	'desata' 'libera' 'desune' 'desenchufa'
	* noun							-> Untie
	* 'a//' creature				-> Untie
	* 'a//' creature 'de' noun		-> Untie
	* noun 'de' noun				-> Untie
;

Extend	only 'desconecta' last
	* noun 'de' noun				-> Untie
;

Verb	'emplea' 'usa' 'utiliza'
	* noun							-> Use
	* noun 'con'/'en' noun			-> Use
;

Verb	'actua' 'interactua'
	* noun							-> Use
	* 'con' noun					-> Use
	* noun 'con'/'en' noun			-> Use
	* 'con' noun 'en' noun			-> Use
	* 'en' noun 'con' noun			-> Use reverse
;
	

Verb	meta 'creditos' 'credits' 'autor' 'author' 'informacion' 'info'
	*								-> Version
;

Verb	'escribe'
	* noun							-> Write
	* 'en'/'el' noun				-> Write
	* noun 'con' noun				-> Write
	* 'en'/'el' noun 'con' noun		-> Write
; VerboIrregular "escribir" with imperativo 'escribe';

Verb	'xyzzy' 'plugh' 'plover' 'abracadabra'
	*								-> Xyzzy
;

!!------------------------------------------------------------------------------

[ DanceSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0, 6:	"Nunca he sido un gran bailarín.";
		1, 7:	"Nunca he sido una gran bailarina.";
		3, 9:	"Nunca hemos sido unos grandes bailarines.";
		4, 10:	"Nunca hemos sido unas grandes bailarinas.";
		}
	2:	switch (gna) {
		0, 6:	"Nunca has sido un gran bailarín.";
		1, 7:	"Nunca has sido una gran bailarina.";
		3, 9:	"Nunca habéis sido unos grandes bailarines.";
		4, 10:	"Nunca habéis sido unas grandes bailarinas.";
		}
	3:	switch (gna) {
		0, 6:	"Nunca ha sido un gran bailarín.";
		1, 7:	"Nunca ha sido una gran bailarina.";
		3, 9:	"Nunca han sido unos grandes bailarines.";
		4, 10:	"Nunca han sido unas grandes bailarinas.";
		}
	4:	switch (gna) {
		0, 6:	"Nunca fui un gran bailarín.";
		1, 7:	"Nunca fui una gran bailarina.";
		3, 9:	"Nunca fuimos unos grandes bailarines.";
		4, 10:	"Nunca fuimos unas grandes bailarinas.";
		}
	5:	switch (gna) {
		0, 6:	"Nunca fuiste un gran bailarín.";
		1, 7:	"Nunca fuiste una gran bailarina.";
		3, 9:	"Nunca fuisteis unos grandes bailarines.";
		4, 10:	"Nunca fuisteis unas grandes bailarinas.";
		}
	6:	switch (gna) {
		0, 6:	"Nunca fue un gran bailarín.";
		1, 7:	"Nunca fue una gran bailarina.";
		3, 9:	"Nunca fueron unos grandes bailarines.";
		4, 10:	"Nunca fueron unas grandes bailarinas.";
		}
	}
];

[ GoDownSub;
	<<Go d_obj>>;
];

[ GoUpSub;
	<<Go u_obj>>;
];

[ IronSub;
	switch (GRAMMATICAL_INFLECTION) {
	1,2,3:	"No tiene sentido intentar hacer algo así.";
	4,5,6:	"No tenía sentido intentar hacer algo así.";
	}
];

[ KnockOnSub;
	switch (GRAMMATICAL_INFLECTION) {
	1,2,3:
		if (noun has door) "Nadie contesta.";
		"No ocurre nada.";
	4,5,6:
		if (noun has door) "Nadie contestó.";
		"No ocurrió nada.";
	}
];

[ MasturbateSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"Ahora no tengo ganas.";
		3,4,9,10:	"Ahora no tenemos ganas.";
		}
	2:	switch (gna) {
		0,1,6,7:	"Ahora no tienes ganas en realidad.";
		3,4,9,10:	"Ahora no teneis ganas en realidad.";
		}
	3:	switch (gna) {
		0,1,6,7:	"Ahora no tiene ganas.";
		3,4,9,10:	"Ahora no tienen ganas.";
		}
	4:	switch (gna) {
		0,1,6,7:	"No tenía ganas.";
		3,4,9,10:	"No teníamos ganas.";
		}
	5:	switch (gna) {
		0,1,6,7:	"No tenías ganas.";
		3,4,9,10:	"No teníais ganas.";
		}
	6:	switch (gna) {
		0,1,6,7:	"No tenía ganas.";
		3,4,9,10:	"No tenían ganas.";
		}
	}
];

[ ReadSub;
	switch (GRAMMATICAL_INFLECTION) {
		1,2,3:	"No hay nada que leer en ", (the) noun, ".";
		4,5,6:	"No había nada que leer en ", (the) noun, ".";
	}
];

[ ShoutSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"Grito con fuerza.";
		3,4,9,10:	"Gritamos con fuerza.";
		}
	2:	switch (gna) {
		0,1,6,7:	"Gritas con fuerza.";
		3,4,9,10:	"Gritais con fuerza.";
		}
	3:	switch (gna) {
		0,1,6,7:	"Grita con fuerza.";
		3,4,9,10:	"Gritan con fuerza.";
		}
	4:	switch (gna) {
		0,1,6,7:	"Grité con fuerza.";
		3,4,9,10:	"Gritamos con fuerza.";
		}
	5:	switch (gna) {
		0,1,6,7:	"Gritaste con fuerza.";
		3,4,9,10:	"Gritasteis con fuerza.";
		}
	6:	switch (gna) {
		0,1,6,7:	"Gritó con fuerza.";
		3,4,9,10:	"Gritaron con fuerza.";
		}
	}
!	"---¡AARRGHH!";
];

[ SitSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0, 6:	"Ya estoy bastante cómodo de pie.";
		1, 7:	"Ya estoy bastante cómoda de pie.";
		3, 9:	"Ya estamos bastante cómodos de pie.";
		4, 10:	"Ya estamos bastante cómodas de pie.";
		}
	2:	switch (gna) {
		0, 6:	"Ya estás bastante cómodo de pie.";
		1, 7:	"Ya estás bastante cómoda de pie.";
		3, 9:	"Ya estáis bastante cómodos de pie.";
		4, 10:	"Ya estáis bastante cómodas de pie.";
		}
	3:	switch (gna) {
		0, 6:	"Ya está bastante cómodo de pie.";
		1, 7:	"Ya está bastante cómoda de pie.";
		3, 9:	"Ya están bastante cómodos de pie.";
		4, 10:	"Ya están bastante cómodas de pie.";
		}
	4:	switch (gna) {
		0, 6:	"Ya estaba bastante cómodo de pie.";
		1, 7:	"Ya estaba bastante cómoda de pie.";
		3, 9:	"Ya estábamos bastante cómodos de pie.";
		4, 10:	"Ya estábamos bastante cómodas de pie.";
		}
	5:	switch (gna) {
		0, 6:	"Ya estabas bastante cómodo de pie.";
		1, 7:	"Ya estabas bastante cómoda de pie.";
		3, 9:	"Ya estabais bastante cómodos de pie.";
		4, 10:	"Ya estabais bastante cómodas de pie.";
		}
	6:	switch (gna) {
		0, 6:	"Ya estaba bastante cómodo de pie.";
		1, 7:	"Ya estaba bastante cómoda de pie.";
		3, 9:	"Ya estaban bastante cómodos de pie.";
		4, 10:	"Ya estaban bastante cómodas de pie.";
		}
	}
];

[ SleepWithSub gna;
	if (noun == player) <<Masturbate>>;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:  "Ni se me pasa por la cabeza algo así ahora.";
		3,4,9,10: "Ni se nos pasa por la cabeza algo así ahora.";
		}
	2:	switch (gna) {
		0,1,6,7:  "En realidad no se te pasaría por la cabeza algo así ahora.";
		3,4,9,10: "En realidad no se os pasaría por la cabeza algo así ahora.";
		}
	3:	switch (gna) {
		0,1,6,7:  "En realidad no se le pasaría por la cabeza algo así ahora.";
		3,4,9,10: "En realidad no se les pasaría por la cabeza algo así ahora.";
		}
	4:	switch (gna) {
		0,1,6,7: 	"No se me hubiese pasado por la cabeza algo así en aquel 
					momento.";
		3,4,9,10:	"No se nos hubiese pasado por la cabeza algo así en aquel 
					momento.";
		}
	5:	switch (gna) {
		0,1,6,7:	"No se te hubiese pasado por la cabeza algo así en aquel 
					momento.";
		3,4,9,10:	"No se os hubiese pasado por la cabeza algo así en aquel 
					momento.";
		}
	6:	switch (gna) {
		0,1,6,7:	"No se le hubiese pasado por la cabeza algo así en aquel 
					momento.";
		3,4,9,10:	"No se les hubiese pasado por la cabeza algo así en aquel 
					momento.";
		}
	}
];

[ StandUpSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"No estoy en ningún sitio del que deba levantarme.";
		3,4,9,10:	"No estamos en ningún sitio del que debamos levantarnos.";
		}
	2:	switch (gna) {
		0,1,6,7:	"No estás en ningún sitio del que debas levantarte.";
		3,4,9,10:	"No estais en ningún sitio del que debais levantaros.";
		}
	3:	switch (gna) {
		0,1,6,7:	"No está en ningún sitio del que deba levantarse.";
		3,4,9,10:	"No están en ningún sitio del que deban levantarse.";
		}
	4:	switch (gna) {
		0,1,6,7:	"No estaba en ningún sitio del que debiese levantarme.";
		3,4,9,10:	"No estábamos en ningún sitio del que debiésemos 
					levantarnos.";
		}
	5:	switch (gna) {
		0,1,6,7:	"No estabas en ningún sitio del que debieses levantarte.";
		3,4,9,10:	"No estabais en ningún sitio del que debieseis levantaros.";
		}
	6:	switch (gna) {
		0,1,6,7:	"No estaba en ningún sitio del que debiese levantarse.";
		3,4,9,10:	"No estaban en ningún sitio del que debiesen levantarse.";
		}
	}
];

[ UntieSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0, 6:	"No estoy seguro de cómo hacerlo.";
		1, 7:	"No estoy segura de cómo hacerlo.";
		3, 9:	"No estamos seguros de cómo hacerlo.";
		4, 10:	"No estamos seguras de cómo hacerlo.";
		}
	2:	switch (gna) {
		0, 6:	"No estás seguro de cómo hacerlo.";
		1, 7:	"No estás segura de cómo hacerlo.";
		3, 9:	"No estáis seguros de cómo hacerlo.";
		4, 10:	"No estáis seguras de cómo hacerlo.";
		}
	3:	switch (gna) {
		0, 6:	"No parece estar seguro de cómo hacerlo.";
		1, 7:	"No parece estar segura de cómo hacerlo.";
		3, 9:	"No parecen estar seguros de cómo hacerlo.";
		4, 10:	"No parecen estar seguras de cómo hacerlo.";
		}
	4:	switch (gna) {
		0, 6:	"No estaba seguro de cómo hacer eso.";
		1, 7:	"No estaba segura de cómo hacer eso.";
		3, 9:	"No estábamos seguros de cómo hacer eso.";
		4, 10:	"No estábamos seguras de cómo hacer eso.";
		}
	5:	switch (gna) {
		0, 6:	"No estabas seguro de cómo hacer eso.";
		1, 7:	"No estabas segura de cómo hacer eso.";
		3, 9:	"No estabais seguros de cómo hacer eso.";
		4, 10:	"No estabais seguras de cómo hacer eso.";
		}
	6:	switch (gna) {
		0, 6:	"No estaba seguro de cómo hacer eso.";
		1, 7:	"No estaba segura de cómo hacer eso.";
		3, 9:	"No estaban seguros de cómo hacer eso.";
		4, 10:	"No estaban seguras de cómo hacer eso.";
		}
	}
];

[ UseSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"Pienso en algún uso para ", (the) noun ,", pero no 
					termino de decidirme por una acción concreta.";
		3,4,9,10:	"Pensamos en algún uso para ", (the) noun ,", pero no 
					terminamos de decidirnos por una acción concreta."; 
		}
	2:	switch (gna) {
		0,1,6,7:	"Piensas en algún uso para ", (the) noun ,", pero no 
					terminas de decidirte por una acción concreta.";
		3,4,9,10:	"Pensais en algún uso para ", (the) noun ,", pero no 
					terminais de decidiros por una acción concreta."; 
		}
	3:	switch (gna) {
		0,1,6,7:	"Piensa en algún uso para ", (the) noun ,", pero no 
					termina de decidirse por una acción concreta.";
		3,4,9,10:	"Piensan en algún uso para ", (the) noun ,", pero no 
					terminan de decidirse por una acción concreta."; 
		}
	4:	switch (gna) {
		0,1,6,7:	"Pensé en algún uso para ", (the) noun ,", pero no 
					terminé de decidirme por una acción concreta.";
		3,4,9,10:	"Pensamos en algún uso para ", (the) noun ,", pero no 
					terminamos de decidirnos por una acción concreta."; 
		}
	5:	switch (gna) {
		0,1,6,7:	"Pensaste en algún uso para ", (the) noun ,", pero no 
					terminaste de decidirte por una acción concreta.";
		3,4,9,10:	"Pensasteis en algún uso para ", (the) noun ,", pero no 
					terminasteis de decidiros por una acción concreta."; 
		}
	6:	switch (gna) {
		0,1,6,7:	"Pensó en algún uso para ", (the) noun ,", pero no 
					terminó de decidirse por una acción concreta.";
		3,4,9,10:	"Pensaron en algún uso para ", (the) noun ,", pero no 
					terminaron de decidirse por una acción concreta."; 
		}
	}
];

[ WriteSub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"Ahora no tengo nada que escribir.";
		3,4,9,10:	"Ahora no tenemos nada que escribir.";
		}
	2:	switch (gna) {
		0,1,6,7:	"Ahora no tienes nada que escribir.";
		3,4,9,10:	"Ahora no teneis nada que escribir.";
		}
	3:	switch (gna) {
		0,1,6,7:	"Ahora no tiene nada que escribir.";
		3,4,9,10:	"Ahora no tienen nada que escribir.";
		}
	4:	switch (gna) {
		0,1,6,7:	"No tenía nada que escribir.";
		3,4,9,10:	"No teníamos nada que escribir.";
		}
	5:	switch (gna) {
		0,1,6,7:	"No tenías nada que escribir.";
		3,4,9,10:	"No teníais nada que escribir.";
		}
	6:	switch (gna) {
		0,1,6,7:	"No tenía nada que escribir.";
		3,4,9,10:	"No tenían nada que escribir.";
		}
	}
];

[ XyzzySub gna;
	gna = GetGNAOfObject(player, true);
	switch (GRAMMATICAL_INFLECTION) {
	1:	switch (gna) {
		0,1,6,7:	"Un rumor silencioso responde a mis palabras.";
		3,4,9,10:	"Un rumor silencioso responde a nuestras palabras.";
		}
	2:	switch (gna) {
		0,1,6,7:	"Un rumor silencioso responde a tus palabras.";
		3,4,9,10:	"Un rumor silencioso responde a vuestras palabras.";
		}
	3:	"Un rumor silencioso responde a sus palabras.";
	4:	switch (gna) {
		0,1,6,7:	"Un rumor silencioso respondió a mis palabras.";
		3,4,9,10:	"Un rumor silencioso respondió a nuestras palabras.";
		}
	5:	switch (gna) {
		0,1,6,7:	"Un rumor silencioso respondió a tus palabras.";
		3,4,9,10:	"Un rumor silencioso respondió a vuestras palabras.";
		}
	6:	"Un rumor silencioso respondió a sus palabras.";
	}
];


