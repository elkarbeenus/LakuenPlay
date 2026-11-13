// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-License-Identifier: GPL-2.0-or-later

#include <QFont>
#include <QIcon>
#include <fmt/ranges.h>
#include "common/scm_rev.h"
#include "ui_aboutdialog.h"
#include "lichii/about_dialog.h"

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent), ui{std::make_unique<Ui::AboutDialog>()} {
    static const std::string build_id = std::string{Common::g_build_id};
    static const std::string lichii_build =
        fmt::format("{} | based on Lichii and Eden | {} | {}", std::string{Common::g_app_label},
                    std::string{Common::g_build_version}, std::string{Common::g_compiler_id});

    const auto override_build =
        fmt::format(fmt::runtime(std::string(Common::g_title_bar_format_idle)), build_id);
    const auto lichii_build_version = override_build.empty() ? lichii_build : override_build;

    ui->setupUi(this);
    // Try and request the icon from Qt theme (Linux?)
    const QIcon lichii_logo = QIcon::fromTheme(QStringLiteral("io.github.zortos293.lichii"),
                                           QIcon(QStringLiteral(":/img/lichii.ico")));
    if (!lichii_logo.isNull()) {
        ui->labelLogo->setPixmap(lichii_logo.pixmap(112));
    }
    // (LaküenPlay) this build's name instead of Lichii's wordmark; the text below credits Lichii
    ui->labelLichii->setText(QString::fromUtf8(Common::g_app_label));
    {
        QFont font = ui->labelLichii->font();
        font.setPointSizeF(font.pointSizeF() * 1.8);
        font.setBold(true);
        ui->labelLichii->setFont(font);
    }
    ui->labelLichii->setAccessibleName(QString::fromUtf8(Common::g_app_label));
    ui->verticalLayout_3->setContentsMargins(32, 28, 32, 20);
    ui->horizontalLayout->setSpacing(32);
    ui->verticalLayout->setSpacing(10);
    ui->labelBuildInfo->setText(
        ui->labelBuildInfo->text().arg(QString::fromStdString(lichii_build_version),
                                       QString::fromUtf8(Common::g_build_date).left(10)));
}

AboutDialog::~AboutDialog() = default;
