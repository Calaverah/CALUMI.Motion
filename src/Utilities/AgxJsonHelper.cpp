#include "stdafx.h"
#include "AgxJsonHelper.h"

QJsonObject colorToJsonObject(const QColor& color)
{
	QJsonObject jColor;
	jColor["r"] = color.red();
	jColor["g"] = color.green();
	jColor["b"] = color.blue();
	jColor["a"] = color.alpha();
	return jColor;
}

QColor JsonObjectToColor(const QJsonObject& jColor)
{
	QColor color;
	QColor defaultColor = Qt::magenta;
	color.setRed(jColor["r"].toInt(defaultColor.red()));
	color.setGreen(jColor["g"].toInt(defaultColor.green()));
	color.setBlue(jColor["b"].toInt(defaultColor.blue()));
	color.setAlpha(jColor["a"].toInt(defaultColor.alpha()));
	return color;
}

static void RecursiveJsonMask(QJsonObject& result, const QJsonObject& source, const QJsonObject& mask)
{
	for (auto it = mask.constBegin(); it != mask.constEnd(); ++it) {
		const QString& key = it.key();

		
		if (source.contains(key)) {
			
			if (it.value().isObject() && source.value(key).isObject()) {
				
				QJsonObject sourceChild = source.value(key).toObject();
				QJsonObject maskChild = it.value().toObject();
				QJsonObject resultChild;
				RecursiveJsonMask(resultChild, sourceChild , maskChild);
				
				result[key] = resultChild;
			}
			else {
				result[key] = source[key];
			}
		}
	}
}

QJsonObject FindJsonOverlaps(const QJsonObject& source, const QJsonObject& mask)
{
	QJsonObject output;
	
	RecursiveJsonMask(output, source, mask);
	
	return output;
}

static QJsonObject RecursiveJsonBuilder(const QStringList& list, const QJsonValue& value, size_t idx = 0)
{
	QJsonObject output;
	if (idx < list.count() - 1)
	{
		output[list.at(idx)] = RecursiveJsonBuilder(list, value, idx + 1);
	}
	else if (idx == list.count() - 1)
	{
		output[list.at(idx)] = value;
	}
	return output;
}

QJsonObject QStringListToQJsonObject(const QStringList& list, const QJsonValue& value)
{
	if (list.isEmpty()) return QJsonObject();

	return RecursiveJsonBuilder(list, value);
	
}

QJsonObject JsonMerge(const QJsonObject& obj1, const QJsonObject& obj2)
{
	QJsonObject output = obj1;

	for (auto item = obj2.constBegin(); item != obj2.constEnd(); ++item)
	{
		output.insert(item.key(), item.value());
	}

	return output;
}

//keeping for unit testing later
static void RunJsonTest()
{
	QJsonObject og;
	{
		og["id"] = 0;
		og["xdata"] = "x";
		QJsonObject iData;
		iData["model-name"] = "Subtraction";
		iData["color"] = "Magenta";
		QJsonObject propSheet;
		propSheet["Event"] = "Start";
		QJsonArray eventList;
		eventList.append("Event01");
		eventList.append("Event02");
		eventList.append("Event03");
		propSheet["Events"] = eventList;
		iData["propertySheet"] = propSheet;
		og["internal-data"] = iData;
	}

	QJsonObject mask;
	{
		mask["id"] = 3;
		QJsonObject iData;
		iData["color"] = "Blue";
		QJsonObject propSheet;
		QJsonArray eventList;
		eventList.append("Event04");
		eventList.append("Event05");
		eventList.append("Event06");
		propSheet["Events"] = eventList;
		iData["propertySheet"] = propSheet;
		mask["internal-data"] = iData;
	}
	auto result = FindJsonOverlaps(og, mask);

	qDebug() << "Source: \n" << og;
	qDebug() << "Mask: \n" << mask;
	qDebug() << "Result: \n" << result;
}
