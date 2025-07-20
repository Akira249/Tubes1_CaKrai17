# Tubes1_CaKrai17

FSM Library
Library ini menyediakan kelas FSM (Finite State Machine) untuk mengelola transisi state sebuah sistem secara terstruktur. 

* Fungsi Utilitas

uint32_t millis()
-> Menghitung waktu saat ini sejak epoch dalam satuan milidetik.
-> Fungsi ini memanfaatkan std::chrono::system_clock untuk memperoleh waktu sekarang dan mengonversinya menjadi milliseconds. Nilai yang dikembalikan digunakan untuk mencatat timestamp heartbeat dan pencatatan riwayat state.

* Konstruktor dan Destruktor

FSM::FSM()
-> Inisialisasi FSM dengan state awal INIT, semua penghitung nol, dan mencatat entry pertama di riwayat.
-> Konstruktor default mengatur currentState menjadi INIT, lastHeartbeat, delay, errorCount, dan moveCount ke 0. Vektor stateHistory dikosongkan lalu ditambahkan tuple pertama (INIT, millis()).

FSM::FSM(uint32_t delay)
-> Sama seperti konstruktor default, tetapi menetapkan nilai delay sesuai argumen.
-> Memberikan fleksibilitas untuk menentukan jeda awal sebelum loop FSM memanggil update, berguna untuk pengujian atau penyesuaian waktu.

FSM::~FSM()
-> Membersihkan stateHistory.
-> Destruktor memastikan semua entry riwayat dilepas dari memori saat objek FSM dihancurkan.

* Getter dan Setter

SystemState getCurrentState() const
-> Mendapatkan state aktif saat ini.
-> Berguna untuk komponen lain yang ingin memeriksa kondisi FSM tanpa mengubahnya.

void transitionToState(SystemState newState)
-> Berpindah ke newState, memperbarui heartbeat, dan mencatat riwayat.
-> Memastikan setiap transisi dicatat dengan timestamp, sehingga memudahkan debugging atau analisis performa.

void setDelay(uint32_t delay) / void getDelay(uint32_t &delay) const
-> Mengonfigurasi atau membaca interval jeda loop FSM.
-> Delay ini digunakan dalam start() untuk menetapkan berapa lama thread tidur setelah setiap update.

void setErrorCount(int count) / int getErrorCount() const
-> Mengatur dan mendapatkan jumlah error yang terjadi.
-> Digunakan dalam penanganan error untuk menentukan kapan harus melakukan shutdown.

void setMoveCount(int count) / int getMoveCount() const
-> Mengatur dan mendapatkan hitungan aksi MOVEMENT.
-> Aksi MOVEMENT dan SHOOTING menggunakan moveCount untuk logika transisi.

vector<pair<SystemState,uint32_t>> getStateHistory() const
-> Mengembalikan riwayat lengkap transisi state.
-> Riwayat ini berupa vector pasangan (state, timestamp), berguna untuk audit atau debugging.

uint32_t getLastHeartbeat() const / void setLastHeartbeat(uint32_t heartbeat)
-> Mengakses atau menetapkan waktu heartbeat terakhir.
-> Heartbeat menunjukkan kapan terakhir kali FSM melakukan update, dapat dipantau untuk mendeteksi hang.

* Loop Utama dan Update

void start()
-> Menjalankan loop FSM hingga state STOPPED tercapai.
-> Fungsi ini memanggil update() berulang setiap satu detik (delay dapat diubah), menjaga FSM bergerak melalui state sesuai logika.

void update()
-> Memanggil fungsi aksi sesuai currentState, lalu mencatat heartbeat dan riwayat.
-> Menggunakan switch untuk memanggil metode performX() yang relevan; setiap iterasi dicatat dalam stateHistory.

* Aksi Tiap State

void performInit()
-> Inisialisasi sistem, set delay default, lalu transisi ke IDLE.
-> Menyiapkan parameter awal dan mencetak status, memulai FSM ke kondisi siaga.

void performProcess()
-> Menampilkan menu user di state IDLE, menerima input, dan melakukan aksi atau transisi.
-> Menggunakan cin untuk membaca pilihan, memungkinkan interaksi sederhana via console.

void performMovement()
-> Melakukan aksi Moving, menaikkan moveCount, lalu transisi ke SHOOTING jika batas tercapai.
-> Batas moveCount >= 3 memicu state SHOOTING; sebaliknya kembali ke IDLE.

void performShooting()
-> Melakukan aksi Shooting, reset moveCount, kembali ke IDLE.
-> Memastikan setiap sesi tembakan dihitung terpisah dari sesi gerakan.

void performCalculation()
-> Jika tidak ada gerakan (moveCount == 0), masuk ERROR; else kembali IDLE.
-> Mencegah kalkulasi tanpa data gerakan, menjaga integritas alur.

void performErrorHandling()
-> Menangani error, increment errorCount, shutdown jika terlalu banyak error.
-> Setelah errorCount > 3, FSM transisi ke STOPPED untuk mencegah loop tak berujung.

void shutdown()
-> Mencetak pesan shutdown dan membersihkan riwayat state.
-> Menandakan akhir siklus FSM dan membebaskan memori riwayat.