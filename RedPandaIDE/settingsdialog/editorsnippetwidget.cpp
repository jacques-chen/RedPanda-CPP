/*
 * Copyright (C) 2020-2022 Roy Qu (royqh1979@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "editorsnippetwidget.h"
#include "ui_editorsnippetwidget.h"
#include "../mainwindow.h"
#include "../codesnippetsmanager.h"
#include "../iconsmanager.h"

#include <QItemSelectionModel>

EditorSnippetWidget::EditorSnippetWidget(const QString& name, const QString& group,
                                         QWidget *parent) :
    SettingsWidget(name,group,parent),
    ui(new Ui::EditorSnippetWidget)
{
    mUpdatingCode = false;
    ui->setupUi(this);
    ui->tblSnippets->setModel(&mModel);
    connect(ui->editCode, &Editor::changed,
            [this] {
        if (mUpdatingCode)
            return;
        QModelIndex index = ui->tblSnippets->currentIndex();
        if (!index.isValid())
            return;
        PCodeSnippet snippet = mModel.snippets()[index.row()];
        snippet->code = ui->editCode->text();
        setSettingsChanged();
    });
    connect(ui->tblSnippets->selectionModel(), &QItemSelectionModel::currentChanged,
            [this] {
        QModelIndex index = ui->tblSnippets->currentIndex();
        if (!index.isValid()) {
            ui->editCode->setEnabled(false);
            ui->editCode->lines()->clear();
        } else {
            mUpdatingCode = true;
            ui->editCode->setEnabled(true);
            PCodeSnippet snippet = mModel.snippets()[index.row()];
            ui->editCode->lines()->setText(snippet->code);
            mUpdatingCode = false;
        }
    });
    connect(ui->editFileTemplate,&Editor::changed,
            this, &SettingsWidget::setSettingsChanged);
}

EditorSnippetWidget::~EditorSnippetWidget()
{
    delete ui;
}

void EditorSnippetWidget::doLoad()
{
    mModel.updateSnippets(pMainWindow->codeSnippetManager()->snippets());
    ui->editFileTemplate->lines()->setText(pMainWindow->codeSnippetManager()->newFileTemplate());
}

void EditorSnippetWidget::doSave()
{
    pMainWindow->codeSnippetManager()->setSnippets(mModel.snippets());
    pMainWindow->codeSnippetManager()->setNewFileTemplate(ui->editFileTemplate->text());
    pMainWindow->codeSnippetManager()->save();
}

void EditorSnippetWidget::on_btnAdd_clicked()
{
    mModel.addSnippet(QString("").arg(getNewFileNumber()),
                      "",
                      "",
                      "",
                      -1);
    ui->tblSnippets->setCurrentIndex(mModel.lastSnippetCaption());
    ui->tblSnippets->edit(mModel.lastSnippetCaption());
}

void EditorSnippetWidget::updateIcons(const QSize &size)
{
    pIconsManager->setIcon(ui->btnAdd,IconsManager::ACTION_MISC_ADD);
    pIconsManager->setIcon(ui->btnRemove,IconsManager::ACTION_MISC_REMOVE);
}


void EditorSnippetWidget::on_btnRemove_clicked()
{
    QModelIndex index = ui->tblSnippets->currentIndex();
    if (!index.isValid())
        return;
    mModel.remove(index.row());
}
