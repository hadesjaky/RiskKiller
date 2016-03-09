#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QMap>
#include "ItemBase.h"

typedef ItemBase* (*Creator)(const QString type, QPointF pos, ItemBase* linker);

class ItemFactory
{
public:
	static ItemFactory & Instance()
	{
		static ItemFactory instance;
		return instance;
	}

	ItemBase* create(QString itemType, QPointF pos, ItemBase* linker = NULL);
	bool registerCreator(QString itemType, Creator creator);

private:
	ItemFactory();
	QMap<QString, Creator> itemCreators;
	Creator creator;
};
#endif
