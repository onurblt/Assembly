# Assembly IDE
Assembly language IDE based on Logic and Computer Design Fundamentals book

# Özellikler

* Dolaylı ve ya Dolaysız ADD,AND,LDA,STA,BUN,BSA,ISZ komutları çalışmaktadır
* CLA,CLE,CMA,CME,CIR,CIL,INC,SPA,SNA,SZA,SNA,SZE,HLT komutları çalışmaktadır
* Girdi/Çıktı komutları(INP,OUT,SKI,SKO,ION,IOF) doğru çalışmayabilir
* Hatalı satırı gösterme
* Yazaçların,Bayrakların adım adım durumunu görme
* Hafızanın durumunu görme
* Hangi satırın çalıştmış olduğunun işaretlenmesi
* Başlatla durmadan çalışma ve ya İlerle ile adım adım çalıştırma
* Sembolik adresleme

# Ekran Görüntüleri

![Screenshot1](ss1.jpg)

![Screenshot2](ss2.jpg)

# Nasıl Kullanılır?


```assembly
BUN x I ; Dolaylı BUN
x,5 
```

```assembly
BUN x ; x e dallan. x e kadar olan kodlar çalışmayacaktır
ADD y ; y yi ekle(y=5)
ADD z ; z yi ekle(z=6)
x,HLT ; x
y, 5 ; y = 5 anlamında
z, 6 ; z = 6 anlamında

```
