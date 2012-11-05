#Keygen De Merde Template par Clubber

#ceci est le fichier makefile
#si vous avez mingwin32 il vous suffit de l'utilisé avec la commande make dans ce 
#répértoire

#Options de compilateur C
CC=gcc

#Nom du compilateur C++
CPP=g++ 

#Options de compilateur C
CFLAGS= -O2 -Os -s -W -Wall -static-libgcc -mwindows -m32
#-mwindows indique à gcc que l'on compile une apli win32 les autres sont des 
#optimisations et les Warnings
#pour avoir une version "debug" dé-commenté la ligne suivante
#CFALGS

#nom du fichier final
EXENAME=Keygen_Test.exe 

#Librairies (si vous en avez besoins...
LIBS=

#Vous devez avoir UPX.exe pour compresser l'executable
#UPX est très stable même si criticable sur la comopression finale
UPXFLAGS= -9 --ultra-brute

#juste pour les vérification par make (non obligatoire...)
HEADER=resource.h
FILEC=main.c
RESOURCE=resource.rc
#ilm n'est pas prévus qu'il y ai 2 fichiers RC
#vous devrez declarer vous même la méthode windres

#INDIQUE LES FICHIERS A COMPILER OBLIGATOIRE
OBJ=main.o resource.o


#Commandes :
#all (make)
all:release

#release (make release)
release: $(OBJ)
	@echo Link des .o dans $(EXENAME)
	$(CC) -o $(EXENAME) $(OBJ) $(CFLAGS) $(LIBS)

#extrem (make extrem) compresse l'executable avec upx (prend un peu de temps)
extrem:release
	@echo Compression UPX de $(EXENAME)
	upx $(EXENAME) $(UPXFLAGS)

#clean (make clean) nettoye les résidus de compilation... (.o)
clean:
	@echo Nettoyage...
	rm *.o
	
#Comandes internes
resource.o: resource.rc $(HEADER) #verifications
	@echo Compilation de resource.rc
	windres resource.rc resource.o -v

%.o: %.c $(HEADER)
	@echo Compilation de $@ à partir de $<
	$(CC) -c $< -o $@ $(CFLAGS)