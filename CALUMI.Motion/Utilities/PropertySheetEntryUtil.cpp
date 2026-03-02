#include "stdafx.h"
#include "PropertySheetEntryUtil.h"

static AgxLineEdit* CreateLineEdit(bool readOnly = true)
{
	AgxLineEdit* lineEdit = new AgxLineEdit();
	
	
	lineEdit->setFixedWidth(200);
	lineEdit->setTextMargins(6, 0, 6, 0);
	

	if(readOnly)
	{
		lineEdit->setReadOnly(true);
		lineEdit->SetUpDoubleClickEdit();
		lineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	}

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateEntry(AgxColumnTypes type, const QStringList& keyPath, QObject* target, bool autoConnect)
{

	if (autoConnect && !target) return nullptr;

	switch (type)
	{
		case AgxColumnTypes::BasicString:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::String, autoConnect);
		case AgxColumnTypes::BasicInteger:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::Integer, autoConnect);
		case AgxColumnTypes::BasicFloat:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::Float, autoConnect);
		case AgxColumnTypes::BasicVector:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::Vector, autoConnect);
		case AgxColumnTypes::BasicBool:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::Boolean, autoConnect);
		case AgxColumnTypes::CustomDropDown:
			return CreateLineEdit();
		case AgxColumnTypes::CustomInteger:
			return CreateIntegerVarEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::CustomFloat:
			return CreateFloatVarEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::CustomVector:
			return CreateVectorVarEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::CustomBool:
			return CreateBooleanVarEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::Event:
			return CreateEventEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::Prefix:
			return CreatePrefixEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::Suffix:
			return CreateSuffixEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::State:
			return CreateStateEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::Action:
			return CreateActionEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::SyncSystem:
			return CreateSyncEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::BasicMultiVar:
			return CreateBasicMuliVarEntry(keyPath, target, autoConnect);
		case AgxColumnTypes::CustomMultiVar:
			return CreateCustomMuliVarEntry(keyPath, target, autoConnect);
		default:
			return CreateBasicStringEntry(keyPath, target, AgxVarType::String, autoConnect);
	}
}


AgxLineEdit* AgxWidgetUtil::CreateBasicStringEntry(const QStringList& keyPath, QObject* target, AgxVarType varType, bool negativeAllowed, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	auto lineEdit = CreateLineEdit(false);

	switch (varType)
	{
		case AgxVarType::Integer:
			{
				auto validator = new QIntValidator();
				if (!negativeAllowed)validator->setBottom(0);
				lineEdit->setValidator(validator);
			}
			break;
		case AgxVarType::Float:
			{
				auto validator = new QDoubleValidator();
				if (!negativeAllowed)validator->setBottom(0);
				lineEdit->setValidator(validator);
			}
			break;
		case AgxVarType::Boolean:
			lineEdit->setReadOnly(true);
			lineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
			lineEdit->SetUpDoubleClickEdit();
			lineEdit->connect(lineEdit, &AgxLineEdit::DoubleClicked, lineEdit, [lineEdit, iAgx, keyPath, autoConnect]() { 
								QString text = lineEdit->text() == "True" ? "False" : "True";
								if (autoConnect && iAgx)
								{
									QJsonObject input;
									input["value"] = text;
									iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
								}
							  
							  });
			if (autoConnect) autoConnect = false;
			break;
		case AgxVarType::Vector:
			lineEdit->setReadOnly(true);
			lineEdit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
			lineEdit->SetUpDoubleClickEdit();
			lineEdit->connect(lineEdit, &AgxLineEdit::DoubleClicked, lineEdit, [lineEdit, iAgx, keyPath, autoConnect]() {
								MultiVariableDialog* dialog = new MultiVariableDialog();
								dialog->SetCustomVector(true, false);
								dialog->DisableComboBox();
								if (dialog->exec() == QDialog::Accepted)
								{
									if (autoConnect && iAgx)
									{
										QJsonObject input;
										input["value"] = dialog->GetValue();
										iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
									}
								}
								dialog->deleteLater();
							  });
			if (autoConnect) autoConnect = false;
			break;
		default:
			break;
	}
	
	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::editingFinished, target, [iAgx, lineEdit, keyPath]() {
		

		if (iAgx)
		{
			QJsonObject input;
			input["value"] = lineEdit->text();
			iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
		}

			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateEventEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	auto lineEdit = CreateLineEdit();

	if(autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetEventEntryDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetText();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateFloatVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();
	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetFloatVariableEntryDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateIntegerVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetIntegerVariableEntryDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateBooleanVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if(autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetBooleanVariableEntryDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateVectorVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if(autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetVectorVariableEntryDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreatePrefixEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if(autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetPrefixDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
			});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateSuffixEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetSuffixDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateSyncEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetSyncVariableDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateStateEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetStateVariableDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateActionEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		auto dialog = DialogPool_SFBGS::GetInstance().GetActionVariableDialog(lineEdit->text());
		if (dialog->exec() == QDialog::Accepted) {
			if (iAgx)
			{
				QJsonObject input;
				input["value"] = dialog->GetValue();
				iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

static AgxColumnTypes GetTypeRequest() {
	QMessageBox typeDialog;
	auto intButton = typeDialog.addButton("Integer", QMessageBox::NoRole);
	auto floatButton = typeDialog.addButton("Float", QMessageBox::NoRole);
	//auto strButton = typeDialog.addButton("String", QMessageBox::NoRole);
	auto boolButton = typeDialog.addButton("Boolean", QMessageBox::NoRole);
	auto vecButton = typeDialog.addButton("Vector", QMessageBox::NoRole);
	auto cancelButton = typeDialog.addButton("Cancel", QMessageBox::RejectRole);

	typeDialog.findChild<QDialogButtonBox*>()->setOrientation(Qt::Vertical);
	typeDialog.findChild<QDialogButtonBox*>()->setMinimumWidth(200);
	
	typeDialog.exec();

	auto button = typeDialog.clickedButton();
	if (button == cancelButton) return AgxColumnTypes::Default;
	else if (button == intButton) return AgxColumnTypes::BasicInteger;
	else if (button == floatButton) return AgxColumnTypes::BasicFloat;
	//else if (button == strButton) return AgxColumnTypes::BasicString;
	else if (button == boolButton) return AgxColumnTypes::BasicBool;
	else if (button == vecButton) return AgxColumnTypes::BasicVector;
}

AgxLineEdit* AgxWidgetUtil::CreateBasicMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {
		
		QJsonObject input;

		auto type = GetTypeRequest();

		switch (type)
		{
			case AgxColumnTypes::BasicString:
				if (iAgx) {
					bool ok = false;
					input["type"] = "BasicString";
					input["value"] = QInputDialog::getText(nullptr, "Input Dialog", "Enter String",QLineEdit::Normal, lineEdit->text(),&ok);
					if (ok)
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
				}
				break;
			case AgxColumnTypes::BasicInteger:
				if (iAgx) {
					bool ok = false;
					auto startVal = lineEdit->text().toInt(&ok);
					if (!ok) startVal = 0;
					ok = false;
					input["type"] = "BasicInteger";
					input["value"] = std::to_string(QInputDialog::getInt(nullptr,"Input Dialog","Enter Integer",startVal,-2147483647,2147483647,1,&ok)).c_str();
					if(ok)
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
				}
				break;
			case AgxColumnTypes::BasicFloat:
				if (iAgx) {
					bool ok = false;
					auto startVal = lineEdit->text().toDouble(&ok);
					if (!ok) startVal = 0.0;
					ok = false;
					input["type"] = "BasicFloat";
					input["value"] = std::to_string(QInputDialog::getDouble(nullptr, "Input Dialog", "Enter Float", startVal, -2.147483647E9, 2.147483647E9, 5, &ok)).c_str();
					if (ok)
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
				}
				break;
			case AgxColumnTypes::BasicVector:
				if (iAgx) {
					MultiVariableDialog* dialog = new MultiVariableDialog();
					dialog->SetCustomVector(true, false);
					dialog->DisableComboBox();
					if (dialog->exec() == QDialog::Accepted)
					{
						QJsonObject input;
						input["type"] = "BasicVector";
						input["value"] = dialog->GetValue();
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
					}
					dialog->deleteLater();
				}
				break;
			case AgxColumnTypes::BasicBool:
				if (iAgx) {
					bool ok = false;
					int startVal = (_stricmp(lineEdit->text().toStdString().c_str(), "True") == 0) ? 1 : 0;
					QStringList list = { "False","True" };
					QJsonObject input;
					input["type"] = "BasicBoolean";
					input["value"] = QInputDialog::getItem(nullptr, "Select Boolean", "Value", list, startVal, false, &ok);
					if(ok)
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
				}
				break;
			default:
				break;
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}

AgxLineEdit* AgxWidgetUtil::CreateCustomMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect)
{
	auto iAgx = dynamic_cast<IAgxEmbedSceneData*>(target);

	AgxLineEdit* lineEdit = CreateLineEdit();

	if (autoConnect)
		target->connect(lineEdit, &AgxLineEdit::DoubleClicked, target, [iAgx, lineEdit, keyPath]() {

		QJsonObject input;

		auto type = GetTypeRequest();

		switch (type)
		{
			case AgxColumnTypes::BasicString:
				if (iAgx) {
					bool ok = false;
					input["type"] = "BasicString";
					input["value"] = QInputDialog::getText(nullptr, "Input Dialog", "Enter String", QLineEdit::Normal, lineEdit->text(), &ok);
					if (ok)
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
				}
				break;
			case AgxColumnTypes::BasicInteger:
			{
				auto idialog = DialogPool_SFBGS::GetInstance().GetIntegerVariableEntryDialog(lineEdit->text());
				if (idialog->exec() == QDialog::Accepted) {
					if (iAgx)
					{
						input["type"] = "CustomInteger";
						input["value"] = idialog->GetValue();
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
					}
				}
			}
				break;
			case AgxColumnTypes::BasicFloat:
			{
				auto fdialog = DialogPool_SFBGS::GetInstance().GetFloatVariableEntryDialog(lineEdit->text());
				if (fdialog->exec() == QDialog::Accepted) {
					if (iAgx)
					{
						input["type"] = "CustomFloat";
						input["value"] = fdialog->GetValue();
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
					}
				}
			}
				break;
			case AgxColumnTypes::BasicVector:
			{
					auto dialog = DialogPool_SFBGS::GetInstance().GetVectorVariableEntryDialog(lineEdit->text());
					if (dialog->exec() == QDialog::Accepted) {
						if (iAgx)
						{
							QJsonObject input;
							input["type"] = "CustomVector";
							input["value"] = dialog->GetValue();
							iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
						}
					}
			}
				break;
			case AgxColumnTypes::BasicBool:
			{
				auto bdialog = DialogPool_SFBGS::GetInstance().GetBooleanVariableEntryDialog(lineEdit->text());
				if (bdialog->exec() == QDialog::Accepted) {
					if (iAgx)
					{
						input["type"] = "CustomBoolean";
						input["value"] = bdialog->GetValue();
						iAgx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
					}
				}
			}
				break;
			default:
				break;
		}
						});

	lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	return lineEdit;
}
