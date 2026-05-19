#pragma once

#pragma warning(push,0)
#include <QWidget>
// ReSharper disable once CppUnusedIncludeDirective
#include <QPushButton>
#pragma warning(pop)
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Widgets/AgxSidebarContent.h"



class SFBGS_SidebarContentItem : public QWidget
{
	Q_OBJECT
public:
	explicit SFBGS_SidebarContentItem(QWidget* parent = nullptr);

	void SetTitle(const QString& str) const { if (_titleBar) _titleBar->setText(str); }
	void SetupOptionalPropertySheet(bool setting, const bool* source, const QStringList& list);
	void SetOptionalPropertySheetState(bool state);

	[[nodiscard]] AgxNodePropertiesWidget* SetupPropertySheet();
	[[nodiscard]] AgxNodePropertiesWidget* GetPropertySheet() const { return _propertySheet; }

	void InsertAdditionalWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = {{{}}});

Q_SIGNALS:
	void SetUpChildDataRefs(const IAgxEmbedSceneData& source);
	void StateChanged();

private:
	QLabel* _titleBar;
	QVBoxLayout* _layout;

	bool _optionalPropertySheet = false;
	QPushButton* _togglePropertySheet;
	AgxNodePropertiesWidget* _propertySheet = nullptr;
};

class SFBGS_SidebarContent : public AgxSidebarContent
{
	Q_OBJECT
public:
	explicit SFBGS_SidebarContent(QWidget* parent = nullptr);


	void SetTitle(const QString& str) const { if (_titleBar) _titleBar->setText(str); }

	void AddContentItem(QWidget* item, unsigned int idx = 0xFFFFFFFF, int stretch = 0, Qt::Alignment alignment = {{{}}}, bool asExtra = false);
	void AddMainItem(QWidget* item, unsigned int idx = 0xFFFFFFFF, int stretch = 0, Qt::Alignment alignment = {{{}}}, bool asExtra = false);

//Q_SIGNALS:
//	void StateChanged();

public slots:
	void OnStateChanged() const;

private:
	QLabel* _titleBar;
	QVBoxLayout* _itemLayout;
	QVBoxLayout* _mainItem;
	QVBoxLayout* _subItems;
	QVBoxLayout* _extraItems;
	QFrame* _frame = nullptr;

};