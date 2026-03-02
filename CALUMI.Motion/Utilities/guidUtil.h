#pragma once
#pragma warning(push,0)
#include <QObject>
#include <QUuid>
#include <QSet>
#pragma warning(pop)

class guidUtil
{
private: 
	guidUtil()
	{

	}

	~guidUtil() = default;

public:
	guidUtil(const guidUtil& other) = delete;
	guidUtil(guidUtil&& other) = delete;

	guidUtil& operator=(const guidUtil& other) = delete;
	guidUtil& operator=(guidUtil&& other) = delete;

	static guidUtil& GetInstance()
	{
		static guidUtil instance;
		return instance;
	}

	bool addGuid(const QString& guid);
	bool addGuid(const QUuid& guid);
	void releaseGuid(const QString& guid);
	void releaseGuid(const QUuid& guid);

private:
	QSet<QUuid> _guids;
};

class guidObject
{
public:
	guidObject();
	virtual ~guidObject();

	QUuid getGuid() const;
	const QUuid* const getGuidRef() const;
	QUuid setGuid(QUuid guid = QUuid().createUuid(), bool keepOldIfInvalid = true);
	QUuid setGuid(QString guid, bool keepOldIfInvalid = true);

private:
	QUuid _guid = QUuid::createUuid();
};