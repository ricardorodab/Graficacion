/* -------------------------------------------------------------------
 * parts.h
 * versión 1.0
 * Copyright (C) 2016 
 * José Ricardo Rodríguez Abreu,
 * Facultad de Ciencias,
 * Universidad Nacional Autónoma de México, Mexico.
 *
 * Este programa es software libre; se puede redistribuir
 * y/o modificar en los términos establecidos por la
 * Licencia Pública General de GNU tal como fue publicada
 * por la Free Software Foundation en la versión 2 o
 * superior.
 *
 * Este programa es distribuido con la esperanza de que
 * resulte de utilidad, pero SIN GARANTÍA ALGUNA; de hecho
 * sin la garantía implícita de COMERCIALIZACIÓN o
 * ADECUACIÓN PARA PROPÓSITOS PARTICULARES. Véase la
 * Licencia Pública General de GNU para mayores detalles.
 *
 * Con este programa se debe haber recibido una copia de la
 * Licencia Pública General de GNU, de no ser así, visite el
 * siguiente URL:
 * http://www.gnu.org/licenses/gpl.html
 * o escriba a la Free Software Foundation Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * -------------------------------------------------------------------
 */
#include <stdlib.h>
#include <math.h>
//Partes del robot.
void base(void);
void cintura(int cinturaInt);
void brazo(int radioInt);
void codo(void);
void mano(int largo, int radioInt);
void nudillo(int largo);
void dedos(float mvm[], int dedosCerrados);
//Objeto a ser movido.
void pelota(double puntosPelota[]);
