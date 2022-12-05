all:
	make -C server FILE=$(FILE) run
	make -C client N=$(N) run 

.PHONY: clean mrproper

clean:
	make -C server clean
	make -C client clean

mrproper: clean
	make -C server mrproper
	make -C client mrproper