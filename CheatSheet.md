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

