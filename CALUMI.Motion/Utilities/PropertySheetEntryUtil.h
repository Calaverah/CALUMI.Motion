#pragma once
#include "Utilities/AgxJsonHelper.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/DialogPool.h>
#include <Widgets/IAgxEmbedSceneData.h>

namespace AgxWidgetUtil {

	AgxLineEdit* CreateEntry(AgxColumnTypes type, const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateBasicStringEntry(const QStringList& keyPath, QObject* target, AgxVarType varType = AgxVarType::String, bool negativeAllowed = true, bool autoConnect = true);

	AgxLineEdit* CreateEventEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateFloatVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateIntegerVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateBooleanVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateVectorVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreatePrefixEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateSuffixEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateSyncEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
	
	AgxLineEdit* CreateStateEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
	
	AgxLineEdit* CreateActionEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateBasicMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEdit* CreateCustomMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
}