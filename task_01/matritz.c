/**
Es soll ein Programm entwickelt werden, dass zwei große Matrizen A (m x p) und B (p x n)
parallel multipliziert. Zu beachten ist, dass m, n und p jeweils wesentlich größer sind als die
Anzahl der einzusetzenden Prozessoren. Zur Kommunikation zwischen den verschiedenen
Prozessen soll MPI/MPICH verwendet werden.
Eine Standardaufgabe beim parallelen Programmieren ist eine möglichst geschickte Aufteilung
des zu lösenden Problems (in diesem Fall der Matrizen), so dass möglichst wenig Kom-
munikation zwischen den einzelnen parallelen Prozessen erforderlich ist. Überlegen Sie sich,
wie eine gute Aufteilung aussehen könnte und welche Daten dann zwischen den beteiligten
Prozessen ausgetauscht werden müssen.
Messen Sie die Laufzeit des Programms auf 1,2 und 4 Prozessoren.
Abzugeben sind:
− eine Kurzbeschreibung des genutzten Ansatzes,
− der dokumentierte Programmcode,
− die Ergebnisse der Laufzeitmessungen und Interpretationen derselben,
− die Bewertung der gewählten Aufteilung bezüglich Last auf den Knoten und
Kommunikation.
Hinweise:
Hinweise und Anweisungen zur Nutzung des Compute-Server des Instituts finden sich im KVV-
Forum zur Veranstaltung.
Nützliche Links:
− Design and Building of Parallel Programs
Buch (online) von Ian Foster, insbesondere Kapitel 8 (MPI)
http://www-unix.mcs.anl.gov/dbpp/
− Beschreibung der MPI Kommandos und Routinen
http://www-unix.mcs.anl.gov/mpi/www/
− Tutorial material on MPI
http://www.mcs.anl.gov/research/projects/mpi/tutorial/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

struct Matrix
{
    int col, lin;
    double **data;
};

struct Matrix get_plane_Matrix(int col, int lin)
{
    struct Matrix ma;
    ma.col = col;
    ma.lin = lin;
    ma.data = NULL;
    return ma;
}

struct Matrix file_to_Matritz(char filename[], int counter)
{
    char path[] = "./matritzen/";
    char buffer[N];
    // ? chain path and name together
    snprintf(buffer, sizeof(buffer), "./matritzen/%s", filename);

    // ? "r" opens file for reading
    FILE *f = fopen(buffer, "r");

    int line_counter = 0;
    int colume;

    if (!f)
    {
        printf("Fehler: Datei '%s' nicht gefunden!\n", filename);
        exit(1);
    }
    else
    {
        printf("--- MATRITZ: %i --- \n", counter);
        char line[N] = "";

        while (fgets(line, sizeof(line), f))
        {
            line_counter++;

            char *ptr = line;
            double value;

            while (fscanf(ptr, "%lf", &value) == 1)
            {
                printf("%f ", value);
            }
            printf("\n");
        }
    }
    fclose(f);

    struct Matrix my_ma = get_plane_Matrix(3, 5);
    return my_ma;
}

int main(int argc, char *argv[])
{
    // secure
    if (argc < 3)
    {
        printf("Fehler: Bitte gebe mindestens 2 Dateinamen an.\n");
        printf("Aufruf: %s <datei1> <datei2>\n", argv[0]);
        return 1;
    }

    // read file's and create matrix
    int i;
    for (i = 1; i <= 2; i++)
    {
        char *filename = argv[i];
        struct Matrix result_02 = file_to_Matritz(filename, i);
    }

    // print for verivy

    // check if doable

    // calculate
    //  -> Multiprogramm

    // print

    return 0;
}