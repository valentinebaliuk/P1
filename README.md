# P1
Problem palaczy tytoniu.
Specyfikacja:
Żeby zrobić skręt potrzebujemy tytonij,zapałki i papier.
Mamy bar w którym N procesy-palaczy. Każdy z nich ma losowy zasób ze zbioru A={tytonij,zapałki,papier}.Mamy też proces-barmena, który pewną ustaloną liczbę razy losje dwa zasoby. Palacze, które mają trzeći zasób potrzebny do zrobienia skrętu biorą te zasoby i robią skręty, a póżniej idą do pokoju dla palących(bufor) w którym jest ograniczona(w naszym programie max 4, ale da się latwo to zmienić) liczba miejsc, czyli jeśli w buforze będzie >= 4 palaczy one się wypiszą z buforu.Palaczy nie mogą się wypisywać z buforu, dopóki te które zrobili skręt z wylosowanych przez barmena zasobów się nie wpiszą i na odwrót.
