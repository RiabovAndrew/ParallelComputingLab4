# ParallelComputingLab4
 
## Отчёт по лабораторной работе №4   

по дисциплине "Параллельные вычисления"   
студента группы ПА-18-2   
Рябова Андрея Дмитриевича   

### Постановка задачи:   

Инструкции:

![image](https://user-images.githubusercontent.com/43186510/114445511-d7c22c00-9bd8-11eb-81c0-3db6f1135344.png)

### Выполнение:   

#### 1. Инициализация данных передачи

Вектор и матрица для передачи.

![image](https://user-images.githubusercontent.com/43186510/114445626-f6282780-9bd8-11eb-8b89-de5696fc72c2.png)

#### 2. Отправка данных с первого потока

В цикле отправка строк матрицы другим потокам:

![image](https://user-images.githubusercontent.com/43186510/114445766-21127b80-9bd9-11eb-9400-6212a6e0e697.png)

#### 3. Прием данных на других потоках

![image](https://user-images.githubusercontent.com/43186510/114445815-31c2f180-9bd9-11eb-8c20-2f0548fe44ec.png)

#### 4. Обратная отправка данных

![image](https://user-images.githubusercontent.com/43186510/114445881-41423a80-9bd9-11eb-97e5-d727a9df059f.png)

#### 5. Прием данных перым потоком

Вывод данных с подсчётом времени:

![image](https://user-images.githubusercontent.com/43186510/114445945-56b76480-9bd9-11eb-9ceb-c537a2cea763.png)

### Результат:

Откроем папку с **exe** проекта: `C:\Users\dorge>cd /d P:\GitHub\ParallelComputingLab4\Code\x64\Debug`

Запустим программу с 4 потокам: `mpiexec -n 4 ParallelComputingLab2.exe`

![image](https://user-images.githubusercontent.com/43186510/114446168-9aaa6980-9bd9-11eb-9ae4-7e0dde5012a8.png)

Кстати, порядок вывода случайный: зависит от того, какой первый поток доступился до **cmd**.
