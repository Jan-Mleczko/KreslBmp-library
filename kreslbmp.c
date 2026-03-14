/* Biblioteka podprogramów do kreœlenia wykresów i rysunków oraz zapisywania
   ich jako pliki graficzne - Jan Mleczko, 2026. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char const kbmp01;  /*Znacznik zgodnoœci wersji przy konsolidacji.*/
extern unsigned short int szstr;
extern unsigned int rstr;
extern char *wskstr;

void csrys () {  /*Czyszczenie strony.*/
	memset (wskstr, 0, rstr);
	}

void ryspt (x, y) {
	/*Rysowanie punktu.*/
	unsigned int i;

	if (x >= 0 && y >= 0 && x < szstr && (i = y * szstr + x) < rstr)
		wskstr[i] = 1;
	}

void ryspka (int x1, int y1, int x2, int y2)
	{  /*Rysowanie wype³nionego prostok¹ta.*/
	char *w, *wk;
	unsigned int s, ip, ik;

	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 > szstr)
		x2 = szstr;
	if (x1 < x2 && y1 < y2) {
		s = x2 - x1;
		if ((ik = y2 * szstr) > rstr)
			ik = rstr;
		if ((ip = y1 * szstr) < ik) {
			w = wskstr + ip + x1;
			wk = wskstr + ik - szstr + x1;
		petla:
			memset (w, 1, s);
			if (w < wk) {
				w += szstr;
				goto petla;
				}
			}
		}
	}

struct Rysodc {
	int x1, y1, dx, dy, i, rx, ry;
	char w;
	};

static void poczro (struct Rysodc *stan, int x1, int y1, int x2, int y2) {
	int dx, dy;

	stan->dx = dx = x2 - (stan->x1 = x1);
	stan->dy = dy = y2 - (stan->y1 = y1);
	stan->rx = x2;
	stan->ry = y2;
	if (abs (dx) < abs (dy))
		stan->w = (stan->i = dy) < 0 ? 9 : 10;
	else	stan->w = (stan->i = dx) < 0 ? 11 : 12;
	}

static krokro (struct Rysodc *stan) {
	int ii;

	switch (stan->w) {
	case 1:
		if (stan->i) {
			ii = ++stan->i;
			stan->rx = ii * stan->dx / stan->dy + stan->x1;
			stan->ry = ii + stan->y1;
			}
		else	stan->w = 0;
		break;
	case 2:
		if (stan->i) {
			ii = --stan->i;
			stan->rx = ii * stan->dx / stan->dy + stan->x1;
			stan->ry = ii + stan->y1;
			}
		else	stan->w = 0;
		break;
	case 3:
		if (stan->i) {
			ii = ++stan->i;
			stan->rx = ii + stan->x1;
			stan->ry = ii * stan->dy / stan->dx + stan->y1;
			}
		else	stan->w = 0;
		break;
	case 4:
		if (stan->i) {
			ii = --stan->i;
			stan->rx = ii + stan->x1;
			stan->ry = ii * stan->dy / stan->dx + stan->y1;
			}
		else	stan->w = 0;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
		stan->w &= 7;
		break;
	default:
		return 0;
		}
	return 1;
	}

void rysodc (x1, y1, x2, y2) {  /*Rysowanie odcinka.*/
	struct Rysodc stan;
	
	poczro (&stan, x1, y1, x2, y2);
	while (krokro (&stan))
		ryspt (stan.rx, stan.ry);
	}

void ryskol (int sx, int sy, unsigned short int prom) {
	/*Rysowanie wype³nionego ko³a.*/
	int x, y, yg, yd;
	unsigned long int promkw, dopkw;

	promkw = (unsigned long int) prom * prom;
	y = 0;
	while (y <= prom) {
		dopkw = promkw - y * y;
		yg = sy - y;
		yd = sy + y;
		x = 0;
		while (x * x <= dopkw) {
			ryspt (sx + x, yg);
			ryspt (sx - x, yg);
			ryspt (sx + x, yd);
			ryspt (sx - x, yd);
			++x;
			}
		++y;
		}
	}

void rysokr (int sx, int sy, unsigned short int prom) {
	/*Rysowanie okrêgu.*/
	int x, y, yg, yd;
	unsigned long int promkw, dopkw;

	promkw = (unsigned long int) prom * prom;
	x = 0;
	y = prom + 1;
	while (y--) {
		dopkw = promkw - y * y;
		yg = sy - y;
		yd = sy + y;
	xwyz:
		ryspt (sx + x, yg);
		ryspt (sx - x, yg);
		ryspt (sx + x, yd);
		ryspt (sx - x, yd);
		if (x * x < dopkw) {
			++x;
			goto xwyz;
			}
		}
	}

void rystka (x1, y1, x2, y2, x3, y3) {  /*Rysowanie wype³nionego trójk¹ta.*/
	int ax, ay, bx, by, cx, cy, rozc12, rozc23, rozc13, oy1, oy2;
	int acbx, acby, wewnx1, wewnx2;
	unsigned int rz;
	struct Rysodc ab, ac, cb;
	char pierw;

	rozc12 = abs (y2 - y1);
	rozc23 = abs (y3 - y2);
	rozc13 = abs (y3 - y1);
	if (rozc12 > rozc23) {
		if (rozc12 > rozc13) {  /*rozc12 najwiêksza*/
			if (y1 < y2) {
				poczro (&ab, x2, y2, x1, y1);
				poczro (&ac, x3, y3, x1, y1);
				poczro (&cb, x2, y2, x3, y3);
				rz = y1;
				}
			else {
				poczro (&ab, x1, y1, x2, y2);
				poczro (&ac, x3, y3, x2, y2);
				poczro (&cb, x1, y1, x3, y3);
				rz = y2;
				}
			}
		else  /*rozc13 najwiêksza*/
			goto rozc13;
		}
	else {
		if (rozc23 > rozc13) {  /*rozc23 najwiêksza*/
			if (y2 < y3) {
				poczro (&ab, x3, y3, x2, y2);
				poczro (&ac, x1, y1, x2, y2);
				poczro (&cb, x3, y3, x1, y1);
				rz = y2;
				}
			else {
				poczro (&ab, x2, y2, x3, y3);
				poczro (&ac, x1, y1, x3, y3);
				poczro (&cb, x2, y2, x1, y1);
				rz = y3;
				}
			}
		else {
		rozc13:  /*rozc13 najwiêksza*/
			if (y1 < y3) {
				poczro (&ab, x3, y3, x1, y1);
				poczro (&ac, x2, y2, x1, y1);
				poczro (&cb, x3, y3, x2, y2);
				rz = y1;
				}
			else {
				poczro (&ab, x1, y1, x3, y3);
				poczro (&ac, x2, y2, x3, y3);
				poczro (&cb, x1, y1, x2, y2);
				rz = y3;
				}
			}
		}
	pierw = 1;
	rz *= szstr;
	while (rz < rstr && krokro (&ab)) {
		if (pierw || ab.ry != oy1) {
			while (pierw || acby == oy2) {
				if (krokro (&ac)) {
					acbx = ac.rx;
					acby = ac.ry;
					}
				else {	krokro (&cb);
					acbx = cb.rx;
					acby = cb.ry;
					}
				pierw = 0;
				}
			if (ab.rx < acbx) {
				wewnx1 = ab.rx;
				wewnx2 = acbx;
				}
			else {	wewnx1 = acbx;
				wewnx2 = ab.rx;
				}
			if (wewnx1 < 0)
				wewnx1 = 0;
			if (++wewnx2 > szstr)
				wewnx2 = szstr;
			memset (wskstr + rz + wewnx1, 1, wewnx2 - wewnx1);
			rz += szstr;
			pierw = 0;
			oy1 = ab.ry;
			oy2 = acby;
			}
		}
	}

static void zppole (FILE *uchwyt, unsigned int rozm, unsigned int wart) {
	while (rozm--) {
		fputc (wart & 255, uchwyt);
		wart >>= 8;
		}
	}

zaprys (char *nazwa) {  /*Zapis rysunku w pliku w formacie BMP.*/
	static unsigned char
			bmn1[2] = {66, 77},
			bmn2[12] = {0, 0, 0, 0, 32, 0, 0, 0, 12, 0, 0, 0},
			bmn3[10] = {1, 0, 1, 0, 255, 255, 255, 0, 0, 0};
	FILE *plik;
	char *wp, *kp;
	unsigned int br, pr, ib, wys;
	int dobrze;
	unsigned char bajt, msc;

	if ((plik = fopen (nazwa, "wb")) == NULL)
		return 0;
	br = (szstr + 7 >> 3) + 3 & ~3;
	wys = rstr / szstr;
	fwrite (bmn1, sizeof bmn1, 1, plik);
	zppole (plik, 4, wys * br + 32);
	fwrite (bmn2, sizeof bmn2, 1, plik);
	zppole (plik, 2, szstr);
	zppole (plik, 2, wys);
	fwrite (bmn3, sizeof bmn3, 1, plik);
	pr = rstr;
	while (pr) {
		pr -= szstr;
		ib = br;
		wp = wskstr + pr;
		kp = wp + szstr;
		do {
			msc = 128;
			bajt = 0;
			do {
				if (wp < kp && *wp++)
					bajt |= msc;
				} while (msc >>= 1);
			fputc (bajt, plik);
			} while (--ib);
		}
	dobrze = !ferror (plik);
	fclose (plik);
	return dobrze;
	}

static unsigned char const *cszuk (unsigned char const *c,
		unsigned char znak) {
	/*Szukanie w czcionce grafiki dla znaku.*/
	unsigned char zc;

	while ((zc = *c++) != znak && zc != '?')
		c += *c;
	return c;
	}

static int cwekt (unsigned char const *c) {
	return *c == 'W' && !c[1];  /*Czy czcionka jest wektorowa?*/
	}

unsigned char *ladcz (char *nazwa) {
	/*£adowanie czcionki z pliku z pe³n¹ weryfikacj¹ dla bezpieczeñstwa.*/
	unsigned char *c, *sygn, znak, sz;
	unsigned int rozm, weryf, przes;
	int odczyt;
	FILE *plik;

	if ((plik = fopen (nazwa, "rb")) == NULL)
		return NULL;
	sygn = "J.M.Font";
	while (znak = *sygn++) {
		if (fgetc (plik) != znak)
			goto zlzamk;
		}
	if ((odczyt = fgetc (plik)) == EOF)
		goto zlzamk;
	rozm = odczyt;
	if ((odczyt = fgetc (plik)) == EOF
	|| (rozm = (rozm << 8) | odczyt) < 2)
		goto zlzamk;
	if ((c = malloc (rozm)) == NULL)
		goto zlzamk;
	if (!fread (c, rozm, 1, plik) || fgetc (plik) != EOF) {
		free (c);
		goto zlzamk;
		}
	fclose (plik);
	if (cwekt (c)) {  /*Weryfikacja dla czcionki wektorowej.*/
		if (rozm < 5)
			goto zlzamk;
		weryf = 2;
		do {
			znak = c[weryf];
			if ((przes = c[weryf + 1]) & 3 != 2
			|| (rozm - weryf < ++przes))
				goto zlzwol;
			przes += weryf;
			sz = c[weryf + 2];
			weryf += 3;
			while (weryf < przes) {
				if (c[weryf] >= sz)
					goto zlzwol;
				weryf += 2;
				}
			weryf = przes;
			} while (znak != '?');
		}
	else {  /*Wefyfikacja dla normalnej czcionki.*/
		weryf = 0;
		do {
			znak = c[weryf];
			przes = c[weryf + 1];
			if (znak != '?')
				++przes;
			if (przes < 2 || rozm - weryf <= przes) {
			zlzwol:
				free (c);
				return NULL;
				}
			weryf += przes;
			} while (znak != '?');
		}
	return c;
zlzamk:
	fclose (plik);
	return NULL;
	}

static unsigned char const czcwew[] = {
#ifndef BEZCZC
	'A',  6, 0160, 0034, 0023, 0034, 0160,
	'B',  4, 0177, 0111, 0066,
	'C',  5, 0076, 0143, 0101, 0101,
	'D',  5, 0177, 0101, 0143, 0076,
	'E',  4, 0177, 0111, 0111,
	'F',  4, 0177, 0011, 0011,
	'G',  5, 0076, 0143, 0111, 0173,
	'H',  4, 0177, 0010, 0177,
	'I',  4, 0101, 0177, 0101,
	'J',  5, 0070, 0140, 0100, 0177,
	'K',  5, 0177, 0010, 0034, 0167,
	'L',  4, 0177, 0100, 0100,
	'M',  6, 0177, 0006, 0010, 0006, 0177,
	'N',  6, 0177, 0006, 0010, 0060, 0177,
	'O',  6, 0076, 0143, 0101, 0143, 0076,
	'P',  5, 0177, 0021, 0033, 0016,
	'R',  5, 0177, 0011, 0031, 0146,
	'S',  5, 0116, 0113, 0151, 0071,
	'T',  4, 0001, 0177, 0001,
	'U',  6, 0077, 0140, 0100, 0140, 0077,
	'W',  6, 0077, 0140, 0070, 0140, 0077,
	'Y',  6, 0007, 0014, 0170, 0014, 0007,
	'Z',  6, 0141, 0121, 0111, 0105, 0103,
	'Q',  6, 0036, 0041, 0061, 0076, 0100,
	'V',  6, 0003, 0074, 0100, 0074, 0003,
	'X',  6, 0101, 0066, 0010, 0066, 0101,
	' ',  3, 0, 0,
	'0',  7, 0076, 0143, 0131, 0105, 0143, 0076,
	'1',  4, 0004, 0002, 0177,
	'2',  5, 0106, 0163, 0131, 0117,
	'3',  4, 0111, 0111, 0066,
	'4',  6, 0030, 0026, 0023, 0177, 0020,
	'5',  4, 0117, 0111, 0061,
	'6',  5, 0070, 0116, 0113, 0061,
	'7',  5, 0101, 0061, 0015, 0003,
	'8',  5, 0066, 0111, 0111, 0066,
	'9',  5, 0106, 0151, 0071, 0016,
	'.',  3, 0300, 0300,
	',',  4, 0200, 0340, 0140,
	':',  3, 0146, 0146,
	'-',  4, 0010, 0010, 0010,
	'+',  4, 0010, 0034, 0010,
	'!',  3, 0300, 0337,
	'(',  4, 0176, 0203, 0201,
	')',  4, 0201, 0203, 0176,
	'x',  5, 0030, 0074, 0074, 0030,
	'=',  7, 0044, 0044, 0044, 0044, 0044, 0044,
	'<',  6, 0010, 0034, 0024, 0066, 0042,
	'>',  6, 0042, 0066, 0024, 0034, 0010,
	'm',  6, 0044, 0156, 0112, 0333, 0221,
	'w',  6, 0221, 0333, 0112, 0156, 0044,
	'r',  7, 0244, 0144, 0064, 0054, 0046, 0045,
	'k',  6, 0140, 0070, 0014, 0070, 0140,
	'a',  6, 0014, 0070, 0140, 0070, 0014,
	'~',  9, 0030, 0014, 0004, 0014, 0030, 0020, 0030, 0014,
	'n',  6, 0034, 0076, 0052, 0052, 0042,
	';',  4, 0200, 0346, 0146,
	'{',  5, 0010, 0034, 0167, 0101,
	'}',  5, 0101, 0167, 0034, 0010,
	'%',  8, 0102, 0045, 0022, 0010, 0044, 0122, 0041,
	'l', 12, 0102, 0045, 0022, 0010, 0044, 0122, 0041, 0000, 0040, 0120,
	0040,
	'p',  6, 0010, 0170, 0010, 0170, 0010,
	'i', 11, 0024, 0024, 0024, 0024, 0024, 0024, 0125, 0066, 0024, 0010,
	'e', 11, 0010, 0014, 0066, 0125, 0024, 0024, 0125, 0066, 0024, 0010,
	's',  9, 0014, 0020, 0040, 0100, 0100, 0040, 0020, 0014,
	'c',  9, 0140, 0020, 0010, 0004, 0004, 0010, 0020, 0140,
	'\\', 4, 0007, 0034, 0160,
	'|',  2, 0177,
	'/',  4, 0160, 0034, 0007,
	'o',  6, 0116, 0173, 0001, 0173, 0116,
	't',  4, 0002, 0005, 0002,
	'd',  6, 0140, 0130, 0114, 0130, 0140,
	0244, 6, 0076, 0013, 0111, 0253, 0276, /*polskie znaki CP852*/
	0217, 5, 0140, 0220, 0226, 0223,
	0250, 6, 0037, 0025, 0125, 0265, 0225,
	0235, 5, 0020, 0177, 0104, 0102,
	0343, 5, 0360, 0040, 0106, 0363,
	0340, 5, 0140, 0226, 0223, 0140,
	0227, 5, 0220, 0250, 0252, 0111,
	0215, 5, 0220, 0326, 0263, 0220,
	0275, 5, 0220, 0326, 0266, 0220,
#endif
	'?',  5, 0006, 0301, 0331, 0016};

void rysnap (int px, int py, char wyrown, unsigned char const *czcion,
		int wielk, unsigned char const *tekst)
	{  /*Pisanie napisu na rysunku.
	      wyrown - wyrównanie: 'L' - do lewej, 'S' - do œrodka,
	      'P' - do prawej.*/
	unsigned char znak, kol;
	unsigned char const *wybieg, *grafzn;
	int x, y, yy, szwier, wkt;
	unsigned int i;

	if (czcion == NULL)
		czcion = czcwew;
	if (wkt = cwekt (czcion))
		czcion += 2;
	else	wielk = 9;
	y = py;
	do {
		wybieg = tekst;
		szwier = 0;
		while ((znak = *wybieg) && znak != '\n') {
			if (wkt)
				szwier += cszuk (czcion, znak)[1] * wielk >> 8;
			else	szwier += *cszuk (czcion, znak);
			++wybieg;
			}
		if (szwier)
			--szwier;
		switch (wyrown) {
		case 'S':
			x = px - (szwier >> 1);
			break;
		case 'P':
			x = px - szwier;
			break;
		default:
			x = px;
			}
		while (tekst < wybieg) {
			grafzn = cszuk (czcion, *tekst++);
			i = *grafzn;
			if (wkt) {  /*Rysowanie znaku z czcionki wektorowej.*/
				while (i >= 4) {
					i -= 4;
					rysodc ((grafzn[i] * wielk >> 8) + x,
					(grafzn[i + 1] * wielk >> 8) + y,
					(grafzn[i + 2] * wielk >> 8) + x,
					(grafzn[i + 3] * wielk >> 8) + y);
					}
				x += grafzn[1] * wielk >> 8;
				}
			else {  /*Rysowanie znaku z czcionki zwyk³ej.*/
				while (--i) {
					kol = *++grafzn;
					yy = y;
					while (kol) {
						if (kol & 1)
							ryspt (x, yy);
						++yy;
						kol >>= 1;
						}
					++x;
					}
				++x;
				}
			}
		y += wielk;
		tekst = wybieg + 1;
		} while (znak);
	}

unsigned int mrznap (unsigned char const *czcion, int wielk,
		unsigned char const *tekst) {
	/*Mierzenie szerokoœci napisu przy danej czcionce
	i (dla czionek wektorowych) jej ¿¹danej wysokoœci.*/
	unsigned char znak;
	unsigned int suma, wynik;
	int wkt;

	if (czcion == NULL)
		czcion = czcwew;
	if (wkt = cwekt (czcion))
		czcion += 2;
	suma = wynik = 0;
	while (znak = *tekst++) {
		if (znak == '\n') {
			if (suma)
				--suma;
			if (suma > wynik)
				wynik = suma;
			suma = 0;
			}
		else {
			if (wkt)
				suma += cszuk (czcion, znak)[1] * wielk >> 8;
			else	suma += *cszuk (czcion, znak);
			}
		}
	if (suma)
		--suma;
	return suma > wynik ? suma : wynik;
	}