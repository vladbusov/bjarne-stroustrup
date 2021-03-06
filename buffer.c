#include <stdio.h>
#include <stdlib.h>
struct buffer{ //кольцевой буфер реализация
	int begin, end, size; //размер, начало, конец
	char *buf; // данные размеров в 1 байт
};
typedef struct buffer buffer;
buffer* create_buffer(int size) //создаем буфер определенного размера
{
	buffer *b = (buffer *)calloc(1, sizeof(b)); // выделяем память под указатель на буфер (8 байт/4 байта)
	b->begin = b->end = 0; // итераторы начала и конца буфера
	b->size = size; // присваеваем размер буферу
	b->buf = (char *)calloc(size, sizeof(b->buf[0])); //выделяем 1 байт для данных
	return b; // возвращаем созданный буфер
	/*
	Функция, подобная malloc() выполняет примерно следующие действия:

просматривает список занятых/свободных областей памяти, размещённых в куче, в поисках свободной области подходящего размера;
в случае нехватки свободной памяти может запросить дополнительную память у ОС;
добавляет найденную область в список занятых областей (или помечает область как занятую);
возвращает указатель на начало области памяти;
если по тем или иным причинам выделить память не удалось, сообщает об ошибке (например, malloc() возвращает NULL).
	*/
}
int write_buffer(buffer *b, void *data, int size) // пишем в буфер данные определенного размера
{
	char *d = (char *)data; //указываем на данные как на байтовый массив
	int i; // объявляем счетчик байтов
	for (i = 0; i < size && b->begin != (b->end + 1) % b->size; ++i) // проверка, есть ли место в буфере
	{
		b->buf[b->end] = d[i]; // вставляем i-тый байт в конец
		b->end = (b->end + 1) % b->size; //передвигаем конец буфера
	}
	return i; //пишем сколько байтов записано в буфер
}
int read_buffer(buffer *b, void *data, int size) //читаем из буфера в данные определенного размера
{
	char *d = (char *)data; // представляем данные как байтовый массив
	int i; // итератор байтового массива
	for (i = 0; i < size && b->begin != b->end; ++i) // считываем каждый байт по кругу
	{
		d[i] = b->buf[b->begin]; // считываем i-ый байт
		b->begin = (b->begin + 1) % b->size; // передвигаем итератор начала
	}
	return i; //возвращаем количество считанного
}
int main(void)
{
	printf("Size of int = %d ; Size of long long = %d \n", sizeof(int), sizeof(long long));

	buffer* b = create_buffer(100); //создаем буфер 100 байтный
	for (int i = 0; i <= 10; ++i){
		printf("Output: %d\n", i);
		write_buffer(b, &i, sizeof(i)); // пишем в буфер по 4 байта 11 цифр = 44 байт информации
	}
	long long p; // обявляем считавающую переменную в стеке 8 байт
	while (read_buffer(b, &p, sizeof(p)) == sizeof(p)) // считываем по 8 байт
	{
		printf("%d ", (int)p); // выводим первые 4 байта из этих 8 таким образом пропуская одно число
		write_buffer(b, &p, sizeof(int)); //пишем обратно только первые 4 байта в итоге пропуская 1 число
	}
	// сначала читаем 0 (1 пропускаем) 2 (3) 4 (5) 6 (7) 8 (9) 10 ... (в массив записалось 0 2 4 6 8 10) + 6*4 = 24 байта/ 44+24= 68/100
	// теперь читаем (0 пропустили, он после 10) 2 (4) 6 (8) 10 (записалось 2 6 10) 3*4 = 12 / 68 + 12 = 80/100
	// опять читаем (2) 6 (10) (пишем 6)
	// ничего не читаем, так как записанная 6 имеет размер 4 байта, а считываются 8, поэтому 6 не считывается 
	system("pause");
	return 0;
}
