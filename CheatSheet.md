## GCC Stack Protector Support

**Status:** Enabled

Bunu araştırdığımda aslında varsayılan olarak GCC gibi derleyicilerin stack protector canary kullandığını öğrendim.
Stack protector canary kısacası koda bir canary değeri giriyor (bu kendi oluşturduğu bir değişken) daha sonrasında
Çalıştırdığı her fonksiyondan önce başka canary değerini baştaki canary değeri ile compare ediyor
Herhangi bir değişilik söz konusu olduğunda bufferoverflow attack olduğunu anlamış oluyormuş.
Varsayılan olarak bu `-fstack-protector-all` enabled olarak geliyor zaten.

```c
void test_function(char* input) {
    char buffer[10];
    strcpy(buffer, input);
}
```

```c
void test_function(char* input) {
    // The compiler automatically adds these parts
    char __canary = __get_canary();  // Canary value is added
    char buffer[10];
    strcpy(buffer, input);
    // The compiler automatically adds this check
    __check_canary(__canary);        // Canary is checked
}
```

---

## Strict User Copy Checks

**Status:** Disabled

Normal şartlarda Linux sistemlerde, kullanıcı seviyesinden (userspace) kernel seviyesine (kernel space) veri 
kopyalanırken çeşitli güvenlik kontrolleri yapılır:

- Bellek adreslerinin geçerliliği
- Buffer boyutlarının kontrolü
- Veri tiplerinin doğruluğu

Bu özellik devre dışı bırakıldığında:

- Kullanıcı programından kernel'a veri kopyalanırken bu kontroller yapılmaz
- Buffer overflow saldırıları daha kolay hale gelir
- Memory corruption zafiyetleri exploit edilebilir hale gelir

---

## Restrict /dev/mem access               

**Status:** Enabled

`/dev/mem` Linux sistemlerinde sistemin fiziksel belleğine doğrudan erişim sağlayan özel bir dosyadır. 
Bu kısıtlama etkinleştirildiğinde, şu anlama gelir:

- Bunun kısıtlanmış olması, normal kullanıcıların doğrudan fiziksel belleğe erişmesini engeller.
- Açık olsaydı, kötü amaçlı yazılımlar bellekte hassas bilgileri okuyabilir veya sistem bütünlüğünü bozabilirdi.

---

## Restrict /dev/kmem access               

**Status:** Enabled

`/dev/kmem`  çekirdek belleğine (kernel memory) erişim sağlar.
Bu kısıtlama etkinleştirildiğinde, şu anlama gelir:

- Kullanıcıların doğrudan çekirdek belleğini değiştirmesini önler.
- Eğer açık olsaydı, bir saldırgan çekirdek belleğini değiştirerek sistem üzerinde tam kontrol sağlayabilirdi.

---

## grsecurity / PaX

**Status:** No GRKERNSEC

Grsecurity, Linux çekirdeği için geliştirilmiş bir güvenlik geliştirme paketidir. Bu paket, işletim sisteminin güvenliğini önemli ölçüde artıran çeşitli özellikler ve yamalar içerir.

PaX ise Grsecurity'nin bir parçasıdır ve özellikle bellek koruması üzerine odaklanır. Temel özellikleri şunlardır:

1. ASLR (Address Space Layout Randomization): Program bellek adreslerini rastgele hale getirerek saldırıları zorlaştırır.

2. MPROTECT: Bellek sayfalarının izinlerini sıkı bir şekilde kontrol eder.

3. PAGEEXEC ve SEGMEXEC: Belleğin yürütülebilir ve yazılabilir olmasını aynı anda engeller.

"No GRKERNSEC" mesajı, sisteminizde Grsecurity/PaX güvenlik özelliklerinin aktif olmadığını gösterir. Bu durum şu anlama gelebilir:

- Linux çekirdeğiniz Grsecurity yamaları ile derlenmiş değil
- Grsecurity özellikleri devre dışı bırakılmış
- Kullandığınız dağıtım Grsecurity'yi desteklemiyor

Grsecurity'nin eksikliği sistemin daha az güvenli olduğu anlamına gelebilir, ancak bu her zaman kritik bir sorun değildir. Birçok modern Linux dağıtımı, kendi güvenlik önlemlerini ve SELinux gibi alternatif güvenlik çözümlerini kullanmaktadır. 

---

## Kernel Heap Hardening

**Status:** No KERNHEAP

Kernel Heap Hardening, Linux çekirdeğinin heap (yığın) bellek yönetimini güçlendiren bir güvenlik özelliğidir. Bu özellik, heap ile ilgili güvenlik açıklarını ve saldırılarını önlemeye yardımcı olur.

Temel özellikleri şunlardır:

1. Heap metadata koruması: Bellek yönetim verilerini manipülasyonlara karşı korur
2. Serbest bırakılan belleğin yeniden kullanımını geciktirme
3. Bellek bloklarının rastgele yerleştirilmesi
4. Double-free ve use-after-free hatalarına karşı koruma

"No KERNHEAP" mesajı, sistemde kernel heap hardening özelliklerinin aktif olmadığını gösterir. Bu durum:

- Heap tabanlı saldırılara karşı sistemin daha savunmasız olabileceğini
- Bellek yönetimi güvenliğinin standart korumalarla sınırlı olduğunu
- Heap exploit'lerinin daha kolay gerçekleştirilebileceğini gösterir.

---

## System-wide ASLR (Address Space Layout Randomization)

**Status:** Disabled (kernel.randomize_va_space = 0)

ASLR, işletim sisteminin bellek güvenliğini artırmak için kullanılan önemli bir özelliktir. Program bellek adreslerini (stack, heap, kütüphaneler vb.) rastgele yerleştirerek çalışır.

kernel.randomize_va_space değeri şu anlamları taşır:

- 0: ASLR tamamen devre dışı
- 1: Kısmi randomizasyon (stack, vdso, shared memory)
- 2: Tam randomizasyon (stack, vdso, shared memory, data segments)

Sistemde ASLR'nin devre dışı olması:
- Return-oriented programming (ROP) saldırılarını kolaylaştırır
- Buffer overflow exploitlerinin başarı şansını artırır
- Bellek tabanlı saldırılara karşı sistemi daha savunmasız hale getirir

Güvenlik için ASLR'nin etkinleştirilmesi (2 değeri ile) önerilir.

---

## Linux Executable Güvenlik Özellikleri

**Mevcut Durum:**
```
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX enabled    No PIE          No RPATH   No RUNPATH
```

### RELRO (RELocation Read-Only)
**Status:** No RELRO

RELRO, Global Offset Table (GOT) için bir koruma mekanizmasıdır. GOT yazma saldırılarına karşı koruma sağlar.
- Full RELRO aktif edilmediğinde GOT üzerinden yapılacak saldırılara karşı savunmasız kalır
- Derleme sırasında `-Wl,-z,relro,-z,now` flag'i ile aktif edilebilir

### Stack Canary
**Status:** No canary found

Stack buffer overflow saldırılarını tespit etmek için kullanılan bir koruma mekanizmasıdır.
- Yukarıda anlatılan stack protector özelliğinin binary'deki durumunu gösterir
- Derleme sırasında `-fstack-protector-all` flag'i ile aktif edilebilir

### NX (No eXecute)
**Status:** Enabled

Stack'in çalıştırılabilir olmasını engelleyen bir güvenlik özelliğidir.
- Stack üzerinde kod çalıştırılmasını engeller
- Buffer overflow saldırılarında shellcode çalıştırılmasını önler
- Varsayılan olarak aktif durumdadır

### PIE (Position Independent Executable)
**Status:** Disabled

ASLR için gerekli olan, programın belleğe rastgele yüklenmesini sağlayan özelliktir.
- Aktif olmadığında program her zaman aynı bellek adresine yüklenir
- Return Oriented Programming (ROP) saldırılarını kolaylaştırır
- Derleme sırasında `-fPIE -pie` flagleri ile aktif edilebilir

### RPATH/RUNPATH
**Status:** Disabled

Dinamik kütüphane arama yollarını belirten özelliklerdir.
- Güvenlik açısından kapalı olması tercih edilir
- Kötü amaçlı kütüphane yüklenmesini engellemek için kapalı tutulmalıdır

**İyileştirme için önerilen derleme komutu:**
```bash
gcc -o program program.c -fstack-protector-all -fPIE -pie -Wl,-z,relro,-z,now
```

---

# x86 Assembly Girişi

## 1. Register'lar (Kayıtçılar)
Intel **x86** mimarisinde 32-bitlik birkaç önemli **register** bulunur. Bunlar, işlemci içinde **veri saklamak** ve **işlem yapmak** için kullanılır.

| Register | Açıklama |
|----------|---------|
| **EAX** (Accumulator) | Ana işlem register'ı. Genellikle fonksiyonların dönüş değeri burada olur. |
| **EBX** (Base) | Çeşitli işlemlerde kullanılan genel amaçlı bir register. |
| **ECX** (Counter) | Döngüler ve sayma işlemleri için kullanılır. |
| **EDX** (Data) | Genellikle büyük çarpma ve bölme işlemlerinde yardımcı register'dır. |
| **EBP** (Base Pointer) | Stack içinde **fonksiyonun başlangıç adresini** tutar. |
| **ESP** (Stack Pointer) | Stack’in en üst noktasını gösterir. |
| **ESI** (Source Index) | Bellekten veri okuma işlemleri için kullanılır. |
| **EDI** (Destination Index) | Belleğe veri yazma işlemlerinde kullanılır. |
| **EIP** (Instruction Pointer) | Çalıştırılacak **bir sonraki komutun adresini** tutar. |

---

## 2. Assembly Komutları

### 🔹 MOV (Taşıma Komutu)
Veriyi bir **register'dan register'a**, **register'dan memory'ye** ya da **memory'den register'a** taşır.

```assembly
mov eax, 0x5       ; EAX register'ına 5 değerini koy
mov ebx, eax       ; EBX register'ına EAX'teki değeri kopyala
mov [ebp-4], eax   ; Stack’te (ebp - 4) adresine EAX değerini yaz
```

### 🔹 CALL (Fonksiyon Çağırma)
Bir fonksiyon çağırmak için kullanılır.

```assembly
call function_name  ; function_name adlı fonksiyona git
```

### 🔹 RET (Return - Fonksiyon Bitirme)
Fonksiyondan geri dönmek için kullanılır.

```assembly
ret  ; Stack’ten dönüş adresini al ve EIP’ye yükle
```

### 🔹 PUSH ve POP (Stack Manipülasyonu)
`push`, bir değeri stack’e ekler, `pop` ise stack’ten bir değeri çıkarır.

```assembly
push eax  ; EAX içeriğini stack’e kaydet
pop ebx   ; Stack’ten bir değer al, EBX’e yükle
```

### 🔹 LEAVE (Fonksiyondan Çıkış)
Stack’i temizleyerek programın düzgün çalışmasını sağlar.

```assembly
leave  ; mov esp, ebp ve pop ebp komutlarını aynı anda yapar
```

---

## 3. Stack Yapısı
Stack, **LIFO (Last In First Out)** prensibiyle çalışır. Her **fonksiyon çağrıldığında stack’e veri eklenir**, fonksiyon bittiğinde ise veriler geri alınır.

### Örnek Stack İşleyişi

1. Fonksiyon çağrıldığında:
   ```assembly
   push ebp      ; Eski EBP’yi stack'e kaydet
   mov ebp, esp  ; Yeni stack frame’i oluştur
   sub esp, 0x10 ; Local değişkenler için stack’te yer aç
   ```

2. Stack yapısı şu hale gelir:
   ```
   ESP -> [local_var3]
          [local_var2]
          [local_var1]
          [EBP eski değeri]  <- EBP buraya işaret ediyor
          [return_address]  <- Fonksiyon dönüş adresi
   ```

3. Fonksiyon bitince stack temizlenir:
   ```assembly
   leave   ; Stack pointer eski haline gelir
   ret     ; Fonksiyondan çıkılır
   ```

---

## 4. Buffer Overflow Mantığı
Bir program stack'te bir değişkenin içine **fazla veri** yazıldığında, fonksiyonun dönüş adresi üzerine yazma (overwrite) gerçekleşebilir.

### Örnek Buffer Overflow:
```c
void vuln() {
    char buffer[32];
    gets(buffer);  // Güvensiz input alıyor
}
```

Stack'teki yerleşim (normal durum):
```
Yüksek Adresler
[return address]     <- Fonksiyonun dönüş adresi
[saved ebp]         <- Kaydedilmiş EBP değeri
[buffer (32 byte)]  <- Bizim char array'imiz
Düşük Adresler
```

Buffer overflow durumu:
```
Yüksek Adresler
[return address]     <- Saldırgan tarafından değiştirilmiş adres
[saved ebp]         <- Taşan veri
[buffer (32 byte)]  <- 32 byte'tan fazla veri girildiğinde yukarı doğru taşar
Düşük Adresler
```

Eğer buffer'a 32 byte'tan fazla veri girilirse, veri yukarı doğru taşarak önce saved ebp'yi, sonra return address'i ezer. Böylece fonksiyon return ettiğinde, program akışı saldırganın istediği adrese yönlendirilmiş olur.

---

## 5. Passwords

- LEVEL-0: `level0`

- LEVEL-1: `1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a`

- LEVEL-2: `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77`

- LEVEL-3: `492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02`

- LEVEL-4: `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa`

- LEVEL-5: `0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a`

- LEVEL-6: `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31`

- LEVEL-7: `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d`

- LEVEL-8: `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9`

- LEVEL-9: `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a`
