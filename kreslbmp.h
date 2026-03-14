#ifndef KRESLBMP
#define KRESLBMP "KreslBmp 1.0, Jan Mleczko"
extern char const kbmp01;
extern void csrys (void);
extern void ryspt (int, int);
extern void ryspka (int, int, int, int);
extern void rysodc (int, int, int, int);
extern void rystka (int, int, int, int, int, int);
extern void ryskol (int, int, unsigned short int);
extern void rysokr (int, int, unsigned short int);
extern int zaprys (char *);
extern unsigned char *ladcz (char *);
extern void rysnap (int, int, char, unsigned char const *, int,
		unsigned char const *);
extern unsigned int mrznap (unsigned char const *, int, unsigned char const *);
#define czyscRysunek csrys
#define rysujPunkt ryspt
#define rysujProstokat ryspka
#define rysujTrojkat rystka
#define rysujOdcinek rysodc
#define rysujKolo ryskol
#define rysujOkrag rysokr
#define zapiszRysunek zaprys
#define ladujCzcionkeZewn ladcz
#define rysujNapis rysnap
#define mierzSzerokNapisu mrznap
#endif