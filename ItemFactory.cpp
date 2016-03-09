#include "ItemFactory.h"

ItemBase* ItemFactory::create(QString itemType, QPointF pos, ItemBase* linker)
{
	creator = itemCreators.find(itemType).value();
	if(creator == NULL)
	{
		return NULL;
	}
	else
	{
		return creator(itemType, pos, linker);
	}
}


bool ItemFactory::registerCreator(QString itemType, Creator creator)
{
	QMap<QString, Creator>::const_iterator counter = itemCreators.find(itemType);

	if (counter == itemCreators.end())
	{
		itemCreators.insert(itemType, creator);
		return true;
	}
	else
	{
		return false;
	}
}

ItemFactory::ItemFactory()
{
	
}
