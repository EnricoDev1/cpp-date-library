#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

class data {
		int giorno, mese, anno;

		bool check() {
			if (mese < 1 || mese > 12) return false;
			if (giorno < 1 || giorno > giornidelmese()) return false;
			if (anno < 1000 || anno > 9999) return false;
			return true;
		}

	public:

		data();
		data(int g, int m, int a);

		int getGiorno() {
			return giorno;
		}
		int getMese() {
			return mese;
		}
		int getAnno() {
			return anno;
		}

		void setGiorno(int g) {
			set(g, mese, anno);
		}

		void setMese(int m) {
			set(giorno, m, anno);
		}

		void setAnno(int a) {
			set(giorno, mese, a);
		}

		void get() {
			scanf("%d%*c%d%*c%d", &giorno, &mese, &anno);
			if (!check()) {
				cout << endl << "Hai inserito una data errata";
				get();
			}
		}

		void set(int g, int m, int a) {
			giorno = g;
			mese = m;
			anno = a;
			if (!check()) {
				cout << endl << "Errore, data ";
				print();
				cout << " non valida";
				exit(0);
			}
		}

		void print() {
			cout << giorno << '/' << mese << '/' << anno;
		}

		bool bisestile() {
			return anno % 4 == 0 && anno % 100 != 0 || anno % 400 == 0;
		}

		int giornidelmese () {
			int giorniMese[12] = {31, bisestile() + 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
			return giorniMese[mese - 1];
		}

		data successivo() {
			if (giorno == 31 && mese == 12) {
				return data(1, 1, anno + 1);
			} else if (giorno == giornidelmese()) {
				return data(1, mese + 1, anno);
			} else {
				return data(giorno + 1, mese, anno);
			}
		}

		data successivo(int n) {
			data app = *this;
			if (n < 0) return precedente(-n);
			for (int i = 0; i < n; i++)
				app = app.successivo();
			return app;
		}

		data precedente() {
			data app = *this;
			if (giorno == mese == 1)
				return data(31, 12, anno - 1);
			else if (giorno == 1) {
				app.mese--;
				return data(app.giornidelmese(), mese - 1, anno);
			} else
				return data(giorno - 1, mese, anno);
		}

		data precedente(int n) {
			data app = *this;
			if (n < 0) return successivo(-n);
			for (int i = 0; i < n; i++)
				app = app.precedente();
			return app;
		}

		int giornoDellaSettimana() {
			int m, k = anno % 100;	 // anno del secolo
			int j = anno / 100;		 // secolo
			j -= 20 - j;
			(mese == 1 || mese == 2) ? m = mese + 12, k-- : m = mese;
			return (giorno + ((13 * (m + 1)) / 5) + k + (k / 4) - j + (j / 4)) % 7;
		}

		string nomeGiornoSettimana() {
			const string days[] = {"domenica", "lunedi", "martedi", "mercoledi", "giovedi", "venerdi", "sabato"};
			return days[giornoDellaSettimana()];
		}

		bool confrontaDate(data d2) {
			if (giorno == d2.giorno && mese == d2.mese && anno == d2.anno)
				return true;
			return false;
		}

		int diffData(data d2) {
			data app = *this;
			bool appMinore = false;
			int count = 0;

			if (app.anno < d2.anno) {
				appMinore = true;
			} else if (app.mese < d2.mese) {
				appMinore = true;
			} else if (app.giorno < d2.giorno) {
				appMinore = true;
			}

			if (!appMinore) {
				data dep = app;
				app = d2;
				d2 = dep;
			}

			while (!app.confrontaDate(d2)) {
				app = app.successivo();
				count++;
			}
			
			return count;
		}

		void stampaCalendario() {
			data app;
			app.set(1, 1, anno);
			string nomeFile = to_string(anno).append(".html");

			ofstream fout(nomeFile);
			const string mesi[] = {"Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre"};
			const string giorni[] = {"Domenica", "Lunedi", "Martedi", "Mercoledi", "Giovedi", "Venerdi", "Sabato"};

			fout << "<!DOCTYPE html><html lang = \"it\"><head><meta charset = \"UTF-8\"><title>Calendario anno " << anno << "</title><style>*{font-family: Arial, Helvetica, sans-serif;}table,td,th {border: 1px solid black; border-collapse: collapse;} #day{background-color: #bfcbe0; font-size: 15px;} td{width: 60px;} tr{height: 36px;} #day:hover{font-size: 17px; font-weight: bold; box-shadow: inset 1px 1px #000000, inset -1px -1px #000000;}</style></head><body><center><h1>Calendario per l'anno " << anno << "</h1><hr></hr>";
			// crea una tabella per ogni mese
			for (int i = 0; i < 12; i++) {
				fout << "<div style=\"display: inline-block; margin-right: 15px; margin-top: 50px; height: 250px;\">";
				fout << "<table><caption style=\"font-size: 25px; font-weight: bold; margin-bottom: 10px;\">" << mesi[i] << "</caption>";

				// stampa i nomi dei giorni della settimana
				for (int c = 0; c < 7; c++)
					fout << "<td style=\"text-align: center;\">" << giorni[c] << "</td>";
				fout << "<tr>";

				int k = app.giornoDellaSettimana(); // salvo il numero del primo giorno del mese
				if (k > 0)
					fout << "<td colspan=" << k << "></td>";

				for (int j = app.giornidelmese(); j > 0; j--) {
					fout << "<td id=\"day\">" << app.giorno << "</td>";
					app = app.successivo();
					if (app.nomeGiornoSettimana() == "domenica" && app.giorno != 1)
						fout << "<tr>";
				}
				fout << "\n</table>\n</div>";
			}
			fout << "\n</center>\n</body>\n</html>";
			fout.close();
			system(nomeFile.c_str());
		}
};


data::data() {
	giorno = 1;
	mese = 1;
	anno = 1970;
}

data::data(int g, int m, int a) {
	set(g, m, a);
}
