#include "actiontoolbar.h"

#include "ui_rbkitmainwindow.h"

ActionToolbar::ActionToolbar(Ui::RbkitMainWindow *_ui)
    : ui(_ui)
{
}

void ActionToolbar::enableProfileActions()
{
    ui->actionComapre_Heapsnapshots->setEnabled(true);
    ui->actionHeap_Snapshot->setEnabled(true);
    ui->action_Trigger_GC->setEnabled(true);
}

void ActionToolbar::disableProfileActions()
{
    ui->actionComapre_Heapsnapshots->setDisabled(true);
    ui->actionHeap_Snapshot->setDisabled(true);
    ui->action_Trigger_GC->setDisabled(true);
}
