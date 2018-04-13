#include <QCoreApplication>
#include <QtCore>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QString* ptr = new QString;
	QHash<int, QString*> hashtable;
	hashtable[2] = ptr;

	if(hashtable.find(2).value() == nullptr)
		std::cout << "nullptr for nonexistent element" << std::endl;

	//return a.exec();
}
