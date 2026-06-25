#define _CRT_SECURE_NO_WARNINGS

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 6 //stavi 105!!!
#define count 3

//zadatak radjen na lab vezbi 
//1. Poslednji proces prvi vrsi upis. Svaki upisuje po 105 elemenata. Koristiti pojedinačne pokazivače na datoteke.
//2. Ponovo otvoriti datoteku. Svaki proces čita podatke upravo napisane koristeći funkcije sa eksplicitnim pomerajem.Proveriti da li je čitanje pravilno izvršeno.
//3. Svaki proces upisuje upravo pročitane podatke u novu datoteku, na sledeći način (pretpostavljajuć da postoje 3 procesa):
//P0 P1 P2    P0 P0 P1 P1 P2 P2   P0 P0 P0 P1 P1 P1 P2 P2 P2 

//Pitanja sa kviza 
    /*
    1.U primeru sa slike, gde svaki proces cita manje delove fajla po round robin principu, 
    efikasnije je iskoristiti veci broj MPI_File_read poziva, 
    za svaki poseban deo podataka u fajlu, nego kreirati pogled na fajl i citanje izvrsiti jednim pozivom.
        Odgovor: netacno

    2. U kojoj liniji isecka koda se nalazi greska 
    (podrazumevati da su promenljive koje se koriste, a nisu definisane, 
    definisane u delu koda koji nije prikazan i nema gresku)?
        Odgovor: greska je u trecoj liniji 
        MPI_File_open(MPI_COMM_WORLD, "/pfs/datafile", CREATE | RDWR, MPI_INFO_NULL, &header); 

    3. Kakav pogled se kreira pozivom sledecih funkcija?
        MPI_Type_contiguous(2, MPI_INT, &t);
        MPI_Type_create_resized(t,0,4*sizeof(int), &type); 
        MPI_Type_commit(&type); 
        MPI_File_set_view(header, 2*sizeof(int), MPI_INT, type, "native", MPI_INFO_NULL); 

        Odgovor: pomeraj za 2 kockice u odnosu na pocetak fajla, po 2 podatka, razmak 4 do sledeceg
    */

int main(int argc, char* argv[])
{
    int rank, size;
    MPI_File fh;
    MPI_Status status;

    int write_buf[N];
    int read_buf[N];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 1.

    for (int i = 0; i < N; i++)
        write_buf[i] = rank * N + i;

    MPI_File_open(MPI_COMM_WORLD,"file1.dat",MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);
    MPI_Offset offset =(size - 1 - rank) * N * sizeof(int);

    MPI_File_seek(fh, offset, MPI_SEEK_SET);
    MPI_File_write(fh,write_buf,N,MPI_INT,&status);
    MPI_File_close(&fh);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        FILE* f = fopen("file1.dat", "rb");
        printf("upis provera: ");

        if (f != NULL)
        {
            int x;

            while (fread(&x, sizeof(int), 1, f))
            {
                printf("%d ", x);
            }

            printf("\n");
            fclose(f);
        }
    }

    // 2.

    MPI_File_open(MPI_COMM_WORLD,"file1.dat",MPI_MODE_RDONLY,MPI_INFO_NULL,&fh);
    MPI_File_read_at(fh,offset, read_buf,N,MPI_INT,&status);
    MPI_File_close(&fh);

    for (int i = 0; i < N; i++)
    {
        if (read_buf[i] != rank * N + i)
        {
            printf("proces %d: greska pri citanju\n", rank);
            break;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // 3.

    MPI_File_open(MPI_COMM_WORLD,"file2.dat",MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

    int* array_of_blocklen = (int*) malloc(count * sizeof(int));
    int* array_of_displ = (int*) malloc(count * sizeof(int));

    int start =  0;
    int countAll =  0;

    for (int i =  0; i < count; i++)
    {
        array_of_blocklen[i] =  i + 1;
        array_of_displ[i] =  start + rank * (i + 1);
        start += size *  (i + 1);
        countAll += (i + 1);
    }

    MPI_Datatype newtype;

    MPI_Type_indexed(count, array_of_blocklen, array_of_displ,MPI_INT, &newtype);
    MPI_Type_commit(&newtype);

    MPI_File_set_view(fh,0,MPI_INT,newtype,"native",MPI_INFO_NULL);
    MPI_File_write_all(fh, read_buf, countAll,MPI_INT,&status);

    MPI_File_close(&fh);

    MPI_Type_free(&newtype);

    free(array_of_blocklen);
    free(array_of_displ);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        FILE* f = fopen("file2.dat", "rb");

        if (f != NULL)
        {
            int x;

            while (fread(&x, sizeof(int), 1, f))
            {
                printf("%d ", x);
            }

            printf("\n");
            fclose(f);
        }
    }

    MPI_Finalize();
    return 0;
}