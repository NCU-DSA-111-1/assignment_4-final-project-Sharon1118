back:back.o
	${CC} -o back back.o ${CFLAGS} ${INC}
main.o:main.c  
	${CC} back.c ${CFLAGS} ${INC} -c    
 
 
clean:                             
	@rm -rf *.o   back
