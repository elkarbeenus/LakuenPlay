// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

#include <QObject>
#include "common/common_types.h"
#include "frontend_common/data_manager.h"
#include "frontend_common/firmware_manager.h"
#include "qt_common/qt_string_lookup.h"

namespace QtCommon::Content {

//
bool CheckGameFirmware(u64 program_id);

enum class FirmwareInstallResult {
    Success,
    NoNCAs,
    FailedDelete,
    FailedCopy,
    FailedCorrupted,
};

inline const QString GetFirmwareInstallResultString(FirmwareInstallResult result) {
    return LOOKUP_ENUM(result, FwInstallSuccess);
}

/**
 * \brief Get a string representation of a result from InstallKeys.
 * \param result The result code.
 * \return A string representation of the passed result code.
 */
inline const QString GetKeyInstallResultString(FirmwareManager::KeyInstallResult result) {
    return LOOKUP_ENUM(result, KeyInstallSuccess);
}

void InstallFirmware(const QString& location, bool recursive);
QString UnzipFirmwareToTmp(const QString& location);

bool CheckKeys();
void InstallFirmware();
void InstallFirmwareZip();

// Keys //
void InstallKeys();

// Content //
void VerifyGameContents(const std::string& game_path);
void VerifyInstalledContents();

void ClearDataDir(FrontendCommon::DataManager::DataDir dir, const std::string& user_id = "");
void ExportDataDir(FrontendCommon::DataManager::DataDir dir, const std::string& user_id = "",
                   const QString& name = QStringLiteral("export"),
                   std::function<void()> callback = {});
void ImportDataDir(FrontendCommon::DataManager::DataDir dir, const std::string& user_id = "",
                   std::function<void()> callback = {});

// Save data transfer (LaküenPlay) //
/// Zips one game's save folder to zip_path. Returns an error text, empty on success.
QString ExportSaveDir(const std::filesystem::path& save_dir, const QString& zip_path);
/// Replaces one game's save folder with a zip's contents (a zip of the folder itself, or of the
/// files in it). The current save is zipped to backup_zip first; nothing is changed when the zip
/// holds no files or the backup fails. Returns an error text, empty on success.
QString ImportSaveDir(const std::filesystem::path& save_dir, const QString& zip_path,
                      const std::filesystem::path& backup_zip);

// Loader //
void configureFilesystemProvider(const std::string& filepath);

// Profiles //
void FixProfiles();
} // namespace QtCommon::Content
