/*
 *Subor:proj2.c
 *Datum 26.11.2014
 *Autor:Dominik Hlavac Duran xhlava42@stud.fit.vutbr.cz
 * Popis:Program sluzi na vypocet tangens pomocou taylorovych radov zretazenych zlomkov 
 *       a na vypocet vzdialenosti a vysky objektov podla zadanych uhlov
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MYABS(x) (x<0 ? -x : x) // ak je odchylka minusova prevedie ju na kladne cislo
#define ITERNUMB 10 // pocet iteracii pri zretazenych zlomkoch ked uz je nulova odchylka

double taylor_tan(double x,unsigned int n) // vypocet tangensu pomocou taylorovych radov
{
 double citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604}; // cleny citatela
 double menovatel[13] = {  1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375}; // cleny menovatela
 unsigned int i;
 double result = x;
 double iterx = x;
 double moc=x*x; // urcenie mocniny
 for (i = 1; i < n+1; i++) // 
    {
     iterx = iterx*moc;
     result = result + (citatel[i]*iterx)/menovatel[i];// vzorec pre vypoceet tan pommocou taylor radov
    }
//printf("%.10e", result);

 return result; // vracia hodnotu tan pomocou taylorovych raddov
}


double cfrac_tan(double x, unsigned int n) // vypocet tangensu pomocou zretazenych zlomkov 
{
 double citatel =1;
 double menovatel;
 double cfranc =0 ;
 int i;
 for (i=n-1; i>=0; i--) //cyklus pre vypocet najvnorenejsieho zlomku .
    {
     menovatel=(i*2+1)/x; //neparne cislo  x --neparne cisla sa zvacsuju
     cfranc=citatel/(menovatel - cfranc); //podiel zlomku
     //printf("%.10e\n",cfranc );
    } 
 return cfranc; // vracia hodnotu tan pomocou zretaz zlomku
}

void helpmsg() // funkcia pre vypis napovedy
{
 printf(
 "Program Iteracne vypocty.\n"
  "Autor: Dominik Hlavac Duran 2014\n"
  "Program sluzi na vypocet tangens cez taylorovych radov a zretazenych zlomkov\n"
  "a vypocita vysku a vzdialenost od meracieho pristroja \n"
  "Pouzitie: proj1 --help   vypise tuto tabulku\n"
    "--tan prorovna presnost vypoctu tangens uhlu tan z matematickej knihovny\n"
    ",Taylorova polynomu a zretazeneho zlomku.\n" 
     "Argumenty N a M udavaju,v ktorych iteracia­ch iteracneho vypoctu ma porovnanie\n"
     " prebie­hat. 0 < N <= M < 14\n"
    "-m vypocita a zmeria vzdialenosti na zaklade zadaneho uhlu v radianoch.\n"
     "argument A udava uhol.Program vypocita  vzdialenost od objektu.0<A<=1.4<pi/2.\n"
     "Ak je zadany, druhy uhol pomocou argument B v radianoch tak program vypocita \n "
     "a vypise aj vysku meraneho objektu. 0 < B <= 1.4 < pi/2\n"
     "Argument -c nastavuje vysku meracieho pristroja c.\n"
     "Vyska c je dana argumentom X (0 < X <= 100).Prednastavena vyska je 1.5 metra.\n"
);
}

int tang(int argc, char * argv[])
{
 double x;
 unsigned int n,i;
 char * err;
 if (argc >5) 
    {
     fprintf(stderr, "ERROR bolo zadanych vela argumentov pre funkciu --tan \n");// ak je zadanych viac argumentov ako je dane pre funkciu vypise chybu 
     return EXIT_FAILURE;
    }
 x = strtod(argv[2],&err);   //osterenie chybneho vstupu
 if (err == argv[2])
    {
     fprintf(stderr,"ERROR zaddana zla hodnota\n");
     return EXIT_FAILURE;
    }
 n = atoi(argv[4]); //posledny prvok rozsahu
 i= atoi(argv[3]);// prvy prvok rozsahu
 if (x>1.4)
    {
     fprintf(stderr, "ERROR zadany zly rozsah parametrov\n");// ak je zadany zly rozsah parametrov tak vypise chybove hlasenie 
     return EXIT_FAILURE;
    }
 if (i>n || n >14)
    {
     fprintf(stderr, "ERROR zadany zly rozsah\n"); // ak je zadany zly rozsah tak vypise chybove hlasenie 
     return EXIT_FAILURE;
    }
 for(;i<=n;i++)
    printf ("%d %e %e %e %e %e\n",i , tan(x), taylor_tan(x,i-1), MYABS(tan(x) - taylor_tan(x,i-1) ) , cfrac_tan(x,i),MYABS((tan(x) - cfrac_tan(x,i)) )); // vypis funkcie --tan
 return EXIT_SUCCESS;
}

int range(int argc, char * argv[])
{
 double x,vyska,vzdialenost;
 unsigned int n;
 char *err;
 if ((strcmp(argv[1],"-m")==0)) // ak je zadany argument -m prebehne vypoccet vzdialenosti  
    {
     if (argc >4) 
        {
         fprintf(stderr, "ERROR bolo zadanych vela argumentov pre funkciu -m\n");// ak je zadanych viac argumentov ako je dane pre funkciu vypise chybu 
         return EXIT_FAILURE;
        }
     else if (argc==3 || argc == 4)
        {
         vyska=1.5;
         n=ITERNUMB; //lebo po 10 iteracii uz nemame odchylku
         x = strtod(argv[2],&err);   //osterenie chybneho vstupu
         if (err == argv[2])
            {
             fprintf(stderr,"ERROR zaddana zla hodnota\n");
             return EXIT_FAILURE;
            }
         printf("%.10e\n",vyska/(cfrac_tan(x,n)) );
         vzdialenost=vyska/cfrac_tan(x,n);// vypocet vzdialenosti pomocou zretazenych zlomkov
         if (argc==4)
            {
             x = strtod(argv[3],&err);   //osterenie chybneho vstupu
             if (err == argv[3])
                {
                 fprintf(stderr,"ERROR zaddana zla hodnota\n");
                 return EXIT_FAILURE;
                }
             printf("%.10e\n",(cfrac_tan(x,n)*vzdialenost+vyska ));// vypocet vysky pomocou zretazenych zlomkov
            }
        } 
    }
 else if  ((strcmp(argv[1],"-c")==0) && (strcmp(argv[3],"-m")==0) )// ak je zadany aj parameter -c tak zmenime vysku meracieho pristroja pri merani vzdialenosti a vysky
    {
     if(argc==5 || argc == 6)
        { 
         vyska = strtod(argv[2],&err);   //osterenie chybneho vstupu
            if (err == argv[2])
                {
                 fprintf(stderr,"ERROR zaddana zla hodnota\n");
                 return EXIT_FAILURE;
                }
         if (vyska>100  || vyska <0 )// osetrenie zadanych hodnout podla zadania
            {
             fprintf(stderr,"ERROR zadany zly rozsah");
             return EXIT_SUCCESS;
            }
         n=ITERNUMB; //lebo po 10 iteracii uz nemame odchylku
         x = strtod(argv[4],&err);   
         if (err == argv[4])
            {
             fprintf(stderr,"ERROR zaddana zla hodnota\n");
             return EXIT_FAILURE;
            }
         printf("%.10e\n",vyska/(cfrac_tan(x,n)) );
         vzdialenost=vyska/cfrac_tan(x,n);// vypocet vzdialenosti objektu pomocou zretazenych zlomkov
         if (argc==6)
            {
             x = strtod(argv[5],&err);  //osterenie chybneho vstupu 
             if (err == argv[5])
                {
                 fprintf(stderr,"ERROR zaddana zla hodnota\n");
                 return EXIT_FAILURE;
                }
             printf("%.10e\n",(cfrac_tan(x,n)*vzdialenost+vyska ));//vypocet vysky objektu pomocou zretazenych zlomkov a predchadzajuceho vypoctu vzdialenosti
            }
        }
    }
    else
    {
     fprintf(stderr, "ERROR neplatny argument, pre napovedu spustite program v tvar /proj1 --help\n"); //ak je zadany argument ini ako su povolee vypise chybu 
     return EXIT_FAILURE;
    }  
  return EXIT_SUCCESS ;
}

int main(int argc, char * argv[])
{
    
 if (argc == 1)
    {
     fprintf(stderr, "ERROR neboli zadane ziade argumenty\n"); // ak nieje zadany argument tak vypisem chybu
     return EXIT_FAILURE;
    } 
 if (argc > 6)
    {
     fprintf(stderr, "ERROR bolo zadanych vela argumentov\n");
     return EXIT_FAILURE;
    } 
 if ((strcmp(argv[1],"--help")==0))// ak je zadany argumet --help vypise napovedu 
    {
     helpmsg();
     return EXIT_SUCCESS;
    }
    else if ((strcmp(argv[1],"--tan")==0) && argc == 5) //ak je zadany argument --tan prebehne vypocet tangensu
    {  
     tang(argc,argv);
     return EXIT_SUCCESS;
    }
 else if ((strcmp(argv[1],"-m")==0) || (strcmp(argv[1],"-c")==0) ) // ak je zadany argument -m prebehne vypoccet vzdialenosti
    {
     range(argc,argv);
     return EXIT_SUCCESS;
    }
 return EXIT_SUCCESS;
}
