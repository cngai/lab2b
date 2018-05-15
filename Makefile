# NAME: Christopher Ngai
# EMAIL: cngai1223@gmail.com
# ID: 404795904

CC = gcc
CFLAGS = -Wall -Wextra

default:
	$(CC) $(CFLAGS) -lprofiler -pthread -g -o lab2_list SortedList.c lab2_list.c

tests: default
	#CYCLES IN BASIC LIST IMPLEMENTATION
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

	#TIMING MUTEX WAITS
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv

	#PERFORMANCE OF PARTITIONED LISTS
	-./lab2_list --threads=1 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=1 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=2 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=4 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=8 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=1 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=4 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=8 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=12 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv
	-./lab2_list --threads=16 --iterations=16 --yield=id --lists=4 >> lab2b_list.csv

	./lab2_list --threads=1 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=s >> lab2b_list.csv

	./lab2_list --threads=1 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=m >> lab2b_list.csv

	./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=s >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=1 --sync=s >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=s >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=s >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=1 --sync=s >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=s >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=s >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=s >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=s >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=s >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=s >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=s >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=s >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=s >>lab2b_list.csv

	./lab2_list --threads=1 --iterations=1000 --lists=1 --sync=m >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=1 --sync=m >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=1 --sync=m >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=1 --sync=m >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=1 --sync=m >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=m >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=m >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=m >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=m >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=m >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=m >>lab2b_list.csv
	./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=m >>lab2b_list.csv
	./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=m >>lab2b_list.csv
	./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >>lab2b_list.csv
	./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >>lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=m >>lab2b_list.csv

profile:
	-rm temp_prof.out
	CPUPROFILE=temp_prof.out ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list temp_prof.out > profile.out
	pprof --list=thread_list ./lab2_list temp_prof.out >> profile.out
	rm temp_prof.out

graphs:
	gnuplot lab2_list.gp

dist: default tests graphs profile
	tar -czf lab2b-404795904.tar.gz README Makefile SortedList.h SortedList.c lab2_list.c lab2b_list.csv profile.out lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png lab2_list.gp

clean:
	rm -rf lab2_list lab2b-404795904.tar.gz