#!/usr/bin/perl

# Script Perl para preprocesar un archivo de texto. Sustituye ciertas etiquetas 
# (*, **, [,]) por funciones definidas dentro del relato para manipular estilo 
# de los textos.

if ($#ARGV != 1) {
	print "Se deben especificar los archivos de entrada y salida del script.";
	print "\n\n";
	exit 4;
}

$input_file = $ARGV[0];
$output_file = $ARGV[1];

# Se abre el archivo de entrada:
open (FILE, "<$input_file") 
	or die "No se pudo abrir el archivo de entrada $input_file: $!\n";
@lines = <FILE>;
close FILE;

# Se abre el archivo de salida:
open (STDOUT, ">$output_file")
	or die "No se pudo abrir el archivo de salida $output_file: $!\n";

for (@lines) {

	# Se hacen las sustituciones (¡el orden importa!):

	next if /^\s*!/;

	s/\*{2,}(.*?)\*{2,}/", (strong) "\1", "/g;
	s/\*(.*?)\*/", (emph) "\1", "/g;

	s/\[(un|una|unos|unas)\s+?(.+?)\]/", (a) \2, "/g;
	s/\[(Un|Una|Unos|Unas)\s+?(.+?)\]/", (A) \2, "/g;
	s/\[(el|la|los|las)\s+?(.+?)\]/", (the) \2, "/g;
	s/\[(El|La|Los|Las)\s+?(.+?)\]/", (The) \2, "/g;
	s/\[(al|a\s+?la|a\s+?los|a\s+?las)\s+?(.+?)\]/", (al) \2, "/g;
	s/\[(Al|A\s+?la|A\s+?los|A\s+?las)\s+?(.+?)\]/", (_Al) \2, "/g;
	s/\[(del|de\s+?la|de\s+?los|de\s+?las)\s+?(.+?)\]/", (del) \2, "/g;
	s/\[(Del|De\s+?la|De\s+?los|De\s+?las)\s+?(.+?)\]/", (_Del) \2, "/g;

	s/\[lista\s+?de\s+?objetos\s+?(en|sobre)\s+?(.+?)\s+?\<(.+?)\>\]/";\nWriteListFrom(child(\2), \3);\nprint "/g;
	s/\[lista\s+?de\s+?objetos\s+?(en|sobre)\s+?(.+?)\]/";\nWriteListFrom(child(\2), ENGLISH_BIT + PARTINV_BIT);\nprint "/g;

	s/\[enlace:\s*?(.+?)\s*?\,\s*?(.+?)\]/";\nlink(\1, "\2");\nprint "/g;
	s/\[(.+?)\]/", (name) \1, "/g;

#	Sustituciones para el sistema de hipervínculos anterior:
#	s/\[(.+?)\]/";\nlink(\1);\nprint "/g;
#	s/link\((.+?)(\:\s*|\,\s*)(.+?)\);/link(\1, "\3");/g;

	print;
}

# Cierra el archivo de salida:
close STDOUT;

