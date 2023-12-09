make:
	g++ -o main main.cpp min_heap.cpp;
	./main;

texto:
	g++ -o main main.cpp min_heap.cpp;
	rm saida.txt;
	./main > saida.txt;

clean:
	rm main