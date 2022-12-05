SRC1 = client.c
EXE1 = client

$(EXE1): $(SRC1)
	gcc $(SRC1) -o $(EXE1) -DPORT=6000

run: $(EXE1)
	for i in $$(seq 1 $(N)); do \
		[ ! -d "Client_$$i" ] && mkdir Client_$$i; \
		gnome-terminal --tab --working-directory=$$PWD/Client_$$i -- ./../$(EXE1); \
	done
clean:
	rm $(EXE1)