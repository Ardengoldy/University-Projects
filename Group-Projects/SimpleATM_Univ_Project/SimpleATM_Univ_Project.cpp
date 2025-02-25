#include <iostream> // utk gunain fitur standar kyk input / output, dllh
#include <algorithm>// utk gunain fitur sortir
#include <iomanip>  // utk gunai fitur yg manipulasi angka atau biar bisa set angka presisi
#include <vector>   // untuk array dinamis (Penambahan indeks (isinya) tidak terbatas, berbeda dgn array biasa yg jumlahnya indeksnya sudah ditentukan)
#include <ctime>    // Untuk cetak waktu transaksi
#include <conio.h>  // Untuk getch() = untuk menahan layar sampai user menekan tombol apa saja, lalu ketika menekan enter akan save
#include <stdlib.h> // Untuk system("cls") atau clearscreen
#include <windows.h>// Untuk sleep atau delay

using namespace std;


// === Deklarasi Global ===
struct Transaction {
    time_t timestamp; // Waktu transaksi
    string type;      // Jenis transaksi
    double amount;    // Nominal transaksi
    string recipient; // Akun penerima (untuk transfer)
};


int pilihan, lanjutkan;
bool bahasa_indonesia = true, kartu_blokir = false;  // Status kartu ATM
string username_input, valid_username, pin_input, rekening_tujuan;

 
double saldo = 0, Jumlah_tf;

// Array utk username, pin, no.rekening dan Histori Transaksi
string username[8] = {"Arden","Azis", "Boneno", "Fadil", "Jo", "Kei", "Nata", "Nathan"};
string pin[8] = {"0001", "0002","0003","0004", "0005", "0006", "0007", "6988"};
string noRek[8] = {"1234000011","1234000012", "1234000013", "1234000014", "1234000015", "1234000016", "1234000017","1234000018"};

vector<Transaction> transactions; // Menyimpan Histori transaksi



// ===== Fungsi Utility =====
void clear_screen() {
    system("cls");
}


// fungsi untuk memvalidasi inputan (pilihan) dengan angka yang sesuai batas
int validateInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            if (bahasa_indonesia) {
                cout << "Maaf, input tidak valid. Silakan coba lagi." << endl;
                cout << "Silakan masukkan angka antara " << min << " dan " << max << ": ";
            } else {
                cout << "Sorry, invalid input. Please try again." << endl;
                cout << "Please enter a number between " << min << " and " << max << ": ";
            }
        } else {
            return input;
        }
    }
}


//Tampilan Loading bar
void loadingBar(int panjang_bar = 15) {
    cout << "Loading: [";
    for (int i = 0; i <= panjang_bar; i++) {
        cout << "=";
        Sleep(100);
        cout << "\rLoading: [" << string(i, '=') << string(panjang_bar - i, ' ') << "] " << (i * 100) / panjang_bar << "%";
    }
    cout << "] Complete!" << endl;
    Sleep(200);
}


// Fungsi untuk  keluar / mematikan program
void exitProgram() {
    int Logout_counter = 5;
    if (bahasa_indonesia) {
        cout << "Program akan keluar dalam 5 detik.\n";
    } else {
        cout << "Program will log out in 5 seconds\n";
    }
    while (Logout_counter >= 1) {
        cout << "\rTime remaining: " << Logout_counter << flush;
        Sleep(1000);
        Logout_counter--;
    }

    clear_screen();
    if (bahasa_indonesia) {

        //Display Keluar Program

        //#!
        cout << "\n==========================================\n";
        cout << "      Terima Kasih telah menggunakan\n";  
        cout << "                 ATM Kelompok 1\n";
        cout << "============================================\n";
        cout << "\nProgram akan dimatikan...\n";

        //#2
        loadingBar();


        //#3 
        cout << "\n==========================================\n";
        cout << "Sampai jumpa lagi! ^_^\n";

    } else {
        //#!
        cout << "\n==========================================\n";
        cout << "              Thank you for using\n";  
        cout << "                 ATM of Group 1\n";
        cout << "============================================\n";
        cout << "\nPreparing to exit the program...\n";

        //#2
        loadingBar();


        //#3 
        cout << "\n==========================================\n";
        cout << "See you next time! ^_^\n";
    }

    exit(0);
}



// Cetak bukti transaksi atau receipt
void cetak_bukti_transaksi(const string &transaksi, double jumlah = 0) {
    if (bahasa_indonesia) {
        cout << "\n=== Resi Transaksi ===\n";
        cout << "Jenis Transaksi: " << transaksi <<endl;
        if (jumlah > 0) {
            cout << "Jumlah: Rp " << fixed << setprecision(2) << jumlah << endl;
        }
        cout << "Saldo Akhir: Rp " << fixed << setprecision(2) << saldo << endl;
        cout << "============================\n";
    } else {
        cout << "\n=== Transaction Receipt ===\n";
        cout << "Transaction type: " << transaksi << endl;
        if (jumlah > 0) {
            cout << "Amount: Rp " << fixed << setprecision(2) << jumlah << endl;
        }
        cout << "Final Balance: Rp " << fixed << setprecision(2) << saldo << endl;
        cout << "============================\n";
    }
}


// fungsi untuk generate waktu / timestamp saat melakukan transaksi
string get_timestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y - %m - %d    %H : %M", ltm);
    return string(timestamp);
}


// Function untuk menambahkan histori transaksi
void add_transaction(const string &type, double amount, const string &recipient = "") {
    Transaction newTransaction;
    newTransaction.timestamp = time(0); // Jam Saat ini
    newTransaction.type = type;
    newTransaction.amount = amount;
    newTransaction.recipient = recipient;
    transactions.push_back(newTransaction); // Add to the vector
}

// FUNGSI UNTUK MENAMPILKAN HISTORI
void lihat_riwayat() {
    clear_screen();

    if (transactions.empty()) {
        cout << (bahasa_indonesia ? "Tidak ada riwayat transaksi.\n" : "No transaction history.\n");
    } else {
        // Sort transactions by date
        sort(transactions.begin(), transactions.end(), [](const Transaction &a, const Transaction &b) {
            return a.timestamp > b.timestamp;
        });

        // Display headers
        cout << "+===========================================================+\n";
        cout << "|                       RIWAYAT TRANSAKSI                   |\n";
        cout << "+===========================================================+\n";

        // Display transactions
        for (const auto &transaction : transactions) {
            cout << "Tanggal: " << ctime(&transaction.timestamp)
                 << "\nTipe: " << transaction.type
                 << "\nJumlah: Rp " << fixed << setprecision(2) << transaction.amount;

            if (!transaction.recipient.empty()) {
                cout << "\nPenerima: " << transaction.recipient;
            }
            cout << "\n=========================\n";
        }

        cout << "+==========================================================+\n";
    }
}

string validasi_noRek(const string &rekening_tujuan) {
    int totaluser = 8; 

    for (int i = 0; i < totaluser; i++) {
        if (noRek[i] == rekening_tujuan) {
            return username[i]; // Return nama pengguna yang terkait jika no.rek sesuai
        }
    }

    return ""; // Return kosong jika no.rek tidak ditemukan
}




// ====== Fungsi CORE Program ======

void tampilkanMenu() {
    clear_screen();
    if (bahasa_indonesia) {
        cout << "+==========================================+\n";
        cout << "|                  MAIN MENU               |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Informasi Saldo                       |\n";
        cout << "| 2. Setor Tunai                           |\n";
        cout << "| 3. Tarik Tunai                           |\n";
        cout << "| 4. Transfer                              |\n";
        cout << "| 5. Mutasi Rekening                       |\n";
        cout << "| 6. Keluar                                |\n";
        cout << "+==========================================+\n";
        cout << "Masukkan pilihan: ";

    } else {
        cout << "+==========================================+\n";
        cout << "|                  MAIN MENU               |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Balance Inquiry                       |\n";
        cout << "| 2. Deposit                               |\n";
        cout << "| 3. Withdraw                              |\n";
        cout << "| 4. Transfer                              |\n";
        cout << "| 5. Transaction History                   |\n";
        cout << "| 6. Exit                                  |\n";
        cout << "+==========================================+\n";
        cout << "Enter Choice: ";
    }
}


// FUNGSI UNTUK MEMELIH BAHASA
void pilih_bahasa() {
    int pilihan_bahasa;
    do {
        cout << "+==========================================+\n";
        cout << "|       Pilih Bahasa / Select Language     |\n";
        cout << "+==========================================+\n";
        cout << "| 1. Bahasa Indonesia                      |\n";            
        cout << "| 2. English                               |\n";
        cout << "+==========================================+\n";
        cout << "Masukkan pilihan / Enter choice: ";

        pilihan_bahasa = validateInput(1,2);

        if (pilihan_bahasa == 1) {
            bahasa_indonesia = true;
            break;
        } else if (pilihan_bahasa == 2) {
            bahasa_indonesia = false;
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan pilih 1 atau 2.\n";
            clear_screen();
        }
    } while (true);
}




// === Fungsi Transaksi ===
void informasi_saldo() {
    loadingBar();
    clear_screen();
    if (bahasa_indonesia) {
        cout << "===== Informasi Saldo=====\n";
        cout << "\nSaldo Anda: Rp " << fixed << setprecision(2) << saldo << endl;
        cout << "===============================\n";
    } else {
        cout << "===== Balance Information =====\n";
        cout << "\nYour Balance: Rp " << fixed << setprecision(2) << saldo << endl;
        cout << "===============================\n";
    }
}

// FUNGSI SETOR TUNAI
void setor_tunai() {
    loadingBar();
    clear_screen();
    double jumlah;
    if (bahasa_indonesia) {
        cout << "Masukkan jumlah setor tunai: Rp ";
    } else {
        cout << "Enter deposit amount: Rp ";
    }

    cin >> jumlah;
    saldo += jumlah;
    loadingBar ();

    if (bahasa_indonesia) {
        cout << "Setoran berhasil. Saldo baru Anda: Rp " << fixed << setprecision(2) << saldo << endl;
        add_transaction("Setor Tunai", jumlah);

    } else {
        cout << "Deposit successful. New saldo: Rp " << fixed << setprecision(2) << saldo << endl;
        add_transaction("Deposit", jumlah);

    }
    cetak_bukti_transaksi(bahasa_indonesia ? "Setor Tunai" : "Deposit", jumlah);
}


// FUNGSI UNTUK PENARIKAN UANG
void tarik_tunai() {
    clear_screen();
    double jumlah;
    if (bahasa_indonesia) {
        cout << "Masukkan jumlah tarik tunai: Rp ";
    } else {
        cout << "Enter withdrawal amount: Rp ";
    }

    cin >> jumlah;
    loadingBar();

    if (jumlah <= saldo) {
        saldo -= jumlah;
        if (bahasa_indonesia) {
            cout << "Penarikan berhasil. Saldo baru Anda: Rp " << fixed << setprecision(2) << saldo << endl;
            add_transaction("Tarik Tunai", jumlah);

        } else {
            cout << "Withdrawal successful. New saldo: Rp " << fixed << setprecision(2) << saldo << endl;
            add_transaction("Withdrawal", jumlah);

        }
        cetak_bukti_transaksi(bahasa_indonesia ? "Tarik Tunai" : "Withdrawal", jumlah);


    } else {
        if (bahasa_indonesia) {
            cout << "Saldo tidak mencukupi.\n";
        } else {
            cout << "Insufficient saldo.\n";
        }
    }
}


//FUNGSI UNTUK TRANSFER
void transfer() {
    system("cls");

    if (bahasa_indonesia){

        cout << "\n====== Transfer ======\n";
        cout << "Masukkan Rekening Tujuan: ";
        cin >> rekening_tujuan;
        cout << "\n======================\n";


        string nama_penerima_tf = validasi_noRek(rekening_tujuan);
        if (nama_penerima_tf.empty()) {
            cout << "Nomor rekening tidak ditemukan. Silakan coba lagi.\n";
            Sleep(3000);
            return;
        }

        cout << "Nama penerima: " << nama_penerima_tf << endl;
        cout << "Masukkan Jumlah Transfer: Rp ";
        cin >> Jumlah_tf;


        if (Jumlah_tf <= 0) {
            cout << "Jumlah tidak valid. Tolong masukkan jumlah positif.\n";
            Sleep(3000);
            return;
        }

        if (Jumlah_tf > saldo) {
            cout << "Saldo anda tidak mencukupi. Harap melakukan setor tunai\n";
            Sleep(3000); //Delay 3 detik
            return;
        } else {
            saldo -= Jumlah_tf; // Deduct amount from saldo
            loadingBar();

            cout << "Transfer ke " << nama_penerima_tf << " dengan jumlah Rp " << fixed << setprecision(2) << Jumlah_tf << " successful.\n";
        }

        add_transaction("Transfer", Jumlah_tf, nama_penerima_tf);


    
    } else {

       cout << "\n====== Transfer ======\n";
        cout << "Enter Recipient Account: ";
        cout << "\n======================\n";
        cin >> rekening_tujuan;


        string nama_penerima_tf = validasi_noRek(rekening_tujuan);
        if (nama_penerima_tf.empty()) {
            cout << "Account number not found. Please try again.\n";
            Sleep(3000);
            return;
        }

        cout << "Recipient Name: " << nama_penerima_tf << endl;
        cout << "Enter Amount to Transfer: Rp ";
        cin >> Jumlah_tf;

        if (Jumlah_tf <= 0) {
            cout << "Invalid amount. Please enter a positive number.\n";
            Sleep(3000);
            return;
        }

        if (Jumlah_tf > saldo) {
            cout << "Insufficient funds for transfer. Please make a deposit before transferring.\n";
            Sleep(3000);
            return;
        } else {
            saldo -= Jumlah_tf; // Deduct amount from saldo
            loadingBar();
            cout << "Transfer to " << nama_penerima_tf << " of Rp " << fixed << setprecision(2) << Jumlah_tf << " successful.\n";
        }
        add_transaction("Transfer", Jumlah_tf, nama_penerima_tf);

    }

    cetak_bukti_transaksi(bahasa_indonesia ? "Transfer" : "Transfer", Jumlah_tf);

}



// ===== Fungsi Inisiasi =====

// FUNGSI UNTUK CEK VALIDASI USERNAME
bool cekUser(string username_input) {
    int totaluser = 8;

    for (int i = 0; i <= totaluser - 1 ; i++) { 
        if (username[i] == username_input) { 
            valid_username += username_input;
            return true;
        }
    }

    return false;
}

//FUNGSI UNTUK CEK VALIDASI PIN
bool validasiUser(string U_Input, string pin_input) {
    int totaluser = 8;

    for (int i = 0; i <= totaluser - 1 ; i++) { 
        if (pin[i] == pin_input && U_Input == username[i]) { 
            return true;
        }
    }

    return false;
}

// FUNGSI LOGIN ATM 
bool login_pin() {
    // Deklarasi lokal
    int percobaan = 0, i, j;

    for (i = 0; i < 1; i++) {
        for (j = 0; j < 3; j++) {
            clear_screen();


            if (bahasa_indonesia) {
                cout << "Masukkan username: ";
                cin >> username_input;
            } else {
                cout << "Enter username: ";
                cin >> username_input;
            }


            if (j == 3) { // pengecekan, jika sudah mencoba 3 kali akan keluar
                break;
            }

            if (cekUser(username_input)) {
                break;
            } else {
                if (bahasa_indonesia) {
                    cout << "Username tidak ditemukan. Coba lagi\n";
                    cout << "\n===============\n";
                    cout << "\rKesempatan tersisa : " << 2-j <<flush; // Menampilkan kesempatan utk input username
                    Sleep(1500); // delay 1,5 detik
                    
                } else {
                    cout << "Username not found. Try again\n";
                    cout << "\n===============\n";
                    cout << "\rAttempts Remaining : " << 2-j <<flush; // Menampilkan kesempatan utk input username
                    Sleep(1500); // delay 1,5 detik


                }
            }
        }

        if (j == 3) { // Kondisi fix untuk cek klo 3 kali salah, maka lngsung out program
            if (bahasa_indonesia) {
                cout << "\n===============\n";
                cout << "Anda telah mencoba 3 kali. Silakan hubungi admin\n";
                exitProgram();
            } else {
                cout << "\n===============\n";
                cout << "You have tried 3 times. Please contact admin\n";
                exitProgram();
            }
        }
        
    }

    while (percobaan < 3) {

        clear_screen();
        
        if (bahasa_indonesia) {
            cout << "Masukkan PIN: ";

            pin_input = "";
            char ch;


            while (true) {
                ch = getch();
                if (ch == '\r') { // '\r' berarti Enter
                    if (pin_input.empty()) {
                        cout << "\nPIN tidak boleh kosong. Tolong masukkan PIN: ";
                        continue;
                    }
                    break;
                } else if (ch == '\b') { // Backspace
                    if (!pin_input.empty()) {
                        cout << "\b \b"; // Menghapus asterisk "*"
                        pin_input.resize(pin_input.size() - 1); // Menghapus karakter terakhir dari `pin_input`
                    }
                } else {
                    cout << '*';
                    pin_input += ch;
                }
            }


        } else {
            cout << "Enter PIN: ";

            pin_input = "";
            char ch;
            while (true) {
                ch = getch();
                if (ch == '\r') {
                    if (pin_input.empty()) {
                        cout << "\nPIN cannot be blank. Please enter again: ";
                        continue;
                    }
                    break;
                } else if (ch == '\b') {
                    if (!pin_input.empty()) {
                        cout << "\b \b";
                        pin_input.resize(pin_input.size() - 1);
                    }
                } else {
                    cout << '*';
                    pin_input += ch;
                }
            }
        }

        if (validasiUser(username_input, pin_input)) {
            return true;
        } else {
            if (bahasa_indonesia) {
                cout << "\nPIN salah. Coba lagi.";
                cout << "\n===============\n";
                cout << "\rKesempatan tersisa : " << 2 - percobaan << flush;
                Sleep(1000); // delay 1 det2ik

            } else {
                cout << "\nIncorrect PIN. Try again.\n";
                cout << "\n===============\n";
                cout << "\rAttempts remaining : " << 2 - percobaan << flush <<endl;
                Sleep(1000); // delay 1 detik
            }

            percobaan++;
            Sleep(400);

        }
    }

    kartu_blokir = true;
    if (bahasa_indonesia) {
        cout << "\nATM Anda diblokir karena 3 kali kesalahan PIN.\n";
    } else {
        cout << "\nYour ATM card is blocked due to 3 incorrect PIN attempts.\n";
    }
    return false;
}


// Fungsi konfirmasi utk Melanjutkan transaksi
bool konfirmasi_lanjut() {
    if (bahasa_indonesia) {
        cout << "Apakah Anda ingin melakukan transaksi lain?" <<endl;
        cout << "1 = Iya\n";
        cout << "0 = Tidak\n";
        cout << "Pilihan anda : ";

        lanjutkan = validateInput(0,1);
    } else {
        cout << "Do you want to perform another transaction?\n"; 
        cout << "1 = Yes\n";
        cout << "0 = No\n";
        cout << "Choose Option : ";
        
        lanjutkan = validateInput(0,1);
    }

    if (lanjutkan == 1){
        return true;
    } else {
        exitProgram();
        return false;
    }
}

//fungsi Menampilkan menu dan menjalankan ATM
void runATM() {
    do {
        tampilkanMenu();

        pilihan = validateInput(1,6);
        switch (pilihan) {
            case 1:
                informasi_saldo();
                break;
            case 2:
                setor_tunai();
                break;
            case 3:
                tarik_tunai();
                break;
            case 4:
                transfer();
                break;
            case 5:
                lihat_riwayat();
                break;
            case 6:
                exitProgram();
                break;
            default:
                cout << (bahasa_indonesia ? "Pilihan tidak valid. Coba lagi.\n" : "Invalid choice. Please try again.\n");
        }
        
    } while (konfirmasi_lanjut());
}

// Fungsi Autentikasi
void loginAndAuthenticate() {
   clear_screen();

    cout << "\n==========================================\n";
    cout << "         Selamat datang / Welcome to\n";  
    cout << "                 ATM Kelompok 1\n";
    cout << "============================================\n";

    cout << "ATM Card detected..." <<endl;
    Sleep(1000);

    cout << "Proccessing ATM Card..." <<endl;
    loadingBar();
    Sleep(1000);

    clear_screen();

    pilih_bahasa();

}

// ===== FUNGSI UTAMA / MENJALANKAN PROGRAM =====
int main() {
    loginAndAuthenticate();

    if (login_pin()) {
        runATM();
    } else {
        cout << (bahasa_indonesia ? "Kartu ATM dikeluarkan.\n" : "ATM card ejected.\n");
    }
    return 0;
}