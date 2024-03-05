
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

struct Proc {
	string NomeProc = "";
	int DurataBurst = 0;
	int ValorePriorita = 0;
	int posAttesa = 0;
};

float attesa(vector<Proc> Processi, int i, int posAttesa) {
	float attesai = 0;
	for (int j = posAttesa; j < i; j++) {
		attesai += Processi[j].DurataBurst;
	}
	return attesai;
}

float FCFS(vector<Proc> Processi, int N) {  // first come first served
	float att = 0;
	for (int i = 0; i < N; i++) {
		cout << Processi[i].NomeProc << " ";
		att += attesa(Processi, i, 0);
	}
	return att;
}

float SJF(vector<Proc> Processi, int N) { //shortest job first 
	float att = 0;
	for (int i = 0; i < N; i++) {
		Proc min = Processi[i];
		for (int j = i + 1; j < N; j++) {
			if (Processi[j].DurataBurst < min.DurataBurst) {
				Processi[i] = Processi[j];
				Processi[j] = min;
				min = Processi[i];
			}
		}
		cout << min.NomeProc << " ";
		att += attesa(Processi, i, 0);
	}
	return att;
}

float PRIO(vector<Proc> Processi, int N) { //priorità, ideantico al SJF, cambia solo il dato su cui viene fatta la valutazione 
	float att = 0;
	for (int i = 0; i < N; i++) {
		Proc min = Processi[i];
		for (int j = i + 1; j < N; j++) {
			if (Processi[j].ValorePriorita < min.ValorePriorita) {
				Processi[i] = Processi[j];
				Processi[j] = min;
				min = Processi[i];
			}
		}
		cout << min.NomeProc << " ";
		att += attesa(Processi, i, 0);
	}
	return att;
}

float RR(vector<Proc> Processi, int N, int Q) { // round robin
	float att = 0;
	for (int i = 0; i < Processi.size(); i++) {
		Proc b = Processi[i];
		if (b.DurataBurst <= Q) {
			att += attesa(Processi, i, b.posAttesa);
			cout << Processi[i].NomeProc << " " << "eseguito per: " << b.DurataBurst << " e;0\n";
		}
		else if (b.DurataBurst > Q) {
			b.DurataBurst = abs(b.DurataBurst - Q);
			Processi[i].DurataBurst = Q; 
			cout << Processi[i].NomeProc << " " << "eseguito per: " << Q << " e;" << b.DurataBurst << "\n";
			att += attesa(Processi, i, b.posAttesa);
			b.posAttesa = i + 1;
			Processi.push_back(b);
		}
			
	}
	return att;
}

int main(int argc, char* argv[]) // per compilare g++ -o eseguibile /home/m.gianvenuti/Scrivania/eseguibile.cpp -std=c++11
{
	string conftxt = argv[1]; //<-quando chiama l'eseguibile deve scrivere separato da spazio il nome del file(compreso percorso) con i processi da leggere
	vector<Proc> Processi;   // es: posizioneeseguibile/eseguibile posizioneconfigurazione.txt/configurazione.txt
	int N = 0, Q = 0;
	string nome, durata, priorita, n, q;

	ifstream myfile(conftxt); //"configurazione.txt" ,conftxt, "/home/m.gianvenuti/Scrivania/configurazione.txt" 
	if (myfile.is_open()) {
		myfile >> n >> n >> q; // prelevo N in questo modo per rimuovere l'hash 
		istringstream tot(n);
		tot >> N;
		istringstream quant(q);
		quant >> Q;
		Proc p;
		while (myfile >> nome >> durata >> priorita) {
			p.NomeProc = nome;
			istringstream dur(durata);
			dur >> p.DurataBurst; 
			istringstream prio(priorita);
			prio >> p.ValorePriorita;
			Processi.push_back(p);
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}
	cout << "Processi\n" << "# " << N << " " << Q << endl;
	for (int i = 0; i < N; i++) {
		cout << Processi[i].NomeProc << " " << Processi[i].DurataBurst << " " << Processi[i].ValorePriorita << endl;
	}
	cout << "\nFCFS ordine di esecuzione dei processi: \n";
	float attfcfs = FCFS(Processi, N);
	cout << "\ntempo medio di attesa FCFS: " << attfcfs / N << "\n";

	cout << "\nSJF ordine di esecuzione dei processi: \n";
	float attsjf = SJF(Processi, N);
	cout << "\ntempo medio di attesa SJF: " << attsjf / N << "\n";

	cout << "\nPRIO ordine di esecuzione dei processi: \n";
	float attprio = PRIO(Processi, N);
	cout << "\ntempo medio di attesa Prio: " << attprio / N << "\n";

	cout << "\nRR ordine di esecuzione dei processi: \n";
	float attrr = RR(Processi, N, Q);
	cout << "tempo medio di attesa RR: " << attrr / N << "\n";

	return 0;
}
