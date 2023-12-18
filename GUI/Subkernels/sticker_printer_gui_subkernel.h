#ifndef STICKERPRINTERGUISUBKERNEL_H
#define STICKERPRINTERGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class StickerPrinterGuiSubkernel : public AbstractGuiSubkernel
{
    Q_OBJECT
public:
    explicit StickerPrinterGuiSubkernel(QObject *parent = nullptr);
};

#endif // STICKERPRINTERGUISUBKERNEL_H
