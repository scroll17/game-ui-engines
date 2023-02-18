# Game & UI engines

## Libraries

* SFML - https://github.com/SFML/SFML
* JSON - https://github.com/nlohmann/json
* XML - https://github.com/zeux/pugixml

### About naming
* __Class__
  * accessing class methods through ___this->___
  * accessing class member variables ___directly___  

### Description
**Все абстрактные классы - реализуют интерфейс и методы**.

Алгоритмы управления: 
- Engine/GameMap - класс для сохранения данных о карте(таил, размер, ...) и её отрисовки и управления игровой картой.
  Предоставляет полезные методы для работы с картой.
  Предоставляет гибкую модель отрисовки (выполнение переданных лямбда функций во время отрисвоки или после), регестрацию елементов которые будут выступать непроходимыми елементами с гибкой настройкой во время их отображения.
- Engine/Player - класс для сохранения данных о игроке (текстура, скорость, позиция, колличество шагов), отрисовкой и управлением игроком. 
  Класс тесно связан с классом GameMap и при выполнении некторых методов Player обращаеться к методам GameMap.
- Engine/Controllers: 
	- StrategiesController - класс который регестрирует стратегии и после чего запускает их в переданной последовательности. Позволяет поставить выполнение стратегий на стоп или начать с указаной стратегии. Класс был использован для построение страниц в игре (загрузка - выбор имени - главное меню - тд...) 
	- FocusController - класс который позволяет определять находиться ли какой-либо из зарегестрированных елементов для текущего окна в фокусе.
	  Реализует паттерн "Singelton"
	- CallSchedulerController: 
		- класс который позволяет запланировать повторяющееся бесконечно или определённое колличество раз действие (план). В качестве дествия (плана) выступает экземпляр класса  SchedulerPlan.
		  Реализует паттерн "Singelton"
		- CallSchedulerController/SchedulerPlan - класс для создания плана. позволяет активировать / деактивировать и запускать план на выполнение.
- Engine/DataUtils: 
	- MousePosition - класс который позволяет получить текущую и предидущую позицию миши.
	  Реализует паттерн "Singelton"
	  
	  
Система UI: 
- Engine/Form: 
	- Form - абстрактный класс который являеться базовым классом для всех остальных классов которые создают window и отображают елементы в window. 
	- Form/Templates:
		- ErrorForm - класс который реализует окно ошибки (соддержит текст ошибки и кнопку выхода)

	- Types: 
		- Element - абстрактный базовый класс который реализует методы отображение, перемещения, позиционирования, размерности елементов в window.
		  Все классы ниже являються предками базового класса Element.
		  Реализует паттерн "Builder (Строитель)" и предки соответственно тоже.
		  
			- Button - класс который реализует елемент "кнопки" и предоставляет методы управления "кнопкой".
				- TextBox - класс который реализует елемент "ввод (input)" и методы по управлению "вводом" и обработкой событий фокусироки + ввода.
			- Text - класс который реализует елемент "текст" и предоставляет методы управления "текстом".
			- ProgressBar - класс который реализует елемент "прогресс бар" и методы управления "баром". Так же реализует событие  after_ready которые срабатывает после загрузки "бара".

	- Objects: 
		- FocusObject - абстрактный класс который реализует методы для обработки логики "елемент в фокусе".
		- ActionObject - абстрактный класс который реализует интерфейс и методоты для обработки таких событий как (установка фокуса, удаление фокуса, кликл по елементу, наведение курсора на елимент, исчезновение курсора с элимента). 

	- ExtendedTypes: 
		- FocusElement - класс который наследуеться от Element и FocusObject
		- ActionElement - класс который наследуеться от Element и ActionObject


Робота з даными: 
- DataTypes: 
	- Exception - класс который наследуеться от std::exception. 
	  Экземпляры класса создаються при возникновении определённых исключительных ситуаций которые описаны в классе Exception в перечислении Type.
	  Предоставляет метод draw_error для отрисовки ошибки в виде окна используя Form/Templates/ErrorForm.
	- Range - класс реализует тип "диапазон" и хранит значение старта и конца диапазона.
	  Предоставляет полезные методы для работы с диапазоном.
	- ExtendedRange - класс наследуеться от Range. Является рассширенным типом "диапазона" который сохраняет отдельное значение от диапазона и предоставляет методы для превращения данных в строку. Используеться для получения данных о двери или переходе на новый этаж.
	- Timer - класс для работы с таймером
	- File:
		- Класс File являеться членом классов FileReader и FileWriter.
		  Предоставляет полезные методы для проверки существования файла и разрешения пути к файлу (относительного или абсолютного).
		- FileReader - класс который читает данные из файла. Может конвертировать прочитанные данные в JSON / XML
		- FileWriter - класс который записывает данные в файл. Может записывать в файл как строки (chunk) так и напрямую из типов JSON / XML
	- Logger - класс для логирования с разными уровнями (DEBUG, INFO, ERROR, ...)
