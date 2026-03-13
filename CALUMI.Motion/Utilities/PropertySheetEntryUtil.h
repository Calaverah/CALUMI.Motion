#pragma once
#include "Utilities/AgxJsonHelper.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/DialogPool.h>
#include <Widgets/IAgxEmbedSceneData.h>

namespace AgxWidgetUtil {

	AgxLineEditContainer* CreateEntry(AgxColumnTypes type, const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateBasicStringEntry(const QStringList& keyPath, QObject* target, AgxVarType varType = AgxVarType::String, bool negativeAllowed = true, bool autoConnect = true);

	AgxLineEditContainer* CreateEventEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateFloatVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateIntegerVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateBooleanVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateVectorVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreatePrefixEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateSuffixEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateSyncEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
	
	AgxLineEditContainer* CreateStateEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
	
	AgxLineEditContainer* CreateActionEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateBasicMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);

	AgxLineEditContainer* CreateCustomMuliVarEntry(const QStringList& keyPath, QObject* target, bool autoConnect = true);
}