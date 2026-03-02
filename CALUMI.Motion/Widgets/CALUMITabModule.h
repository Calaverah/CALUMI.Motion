#pragma once

#pragma warning(push,0)
#include <QWidget>
#include <qboxlayout.h>
#include <qsplitter.h>


#pragma warning (pop)

class CALUMITabModule : public QWidget
{
	Q_OBJECT
public:
	CALUMITabModule(QWidget* content, QFutureWatcher<void>* watcher = nullptr, int current = 0, int progressShare = 1000, QWidget* parent = nullptr);

	QWidget* GetMainWidget() const;

private:
	void CALUMITabModuleConstructorHelper(QWidget* content, QWidget* parent);

public slots:

	//Adds Item to Scroll Area
	void AddSideBarItem_Right(QWidget* item);
	void AdjustSize_Right();
	void SetSidebarVisibility_Right(bool show = true);

	//Only One Item Is Show At A Time
	void SetSideBarItem_Left(QWidget* item, bool show = true);
	void CloseSideBarItem_Left();
	bool GetSideBarVisibility_Left() const;

	void ResetSideBar_Left();
	void ResetSideBar_Right();

Q_SIGNALS:
	void statusUpdate(float loadPercentage, const QString& message = QString());
	
protected:
	QVBoxLayout* _topLayout;
	QSplitter* _centralLayout;
	QWidget* _mainWidget;

	QWidget* _leftAreaParent;
	QVBoxLayout* _leftAreaParentLayout;
	QScrollArea* _rightScrollArea;
	QPointer<QWidget> _rightWidget;
	QVBoxLayout* _rightScrollLayout;

};

