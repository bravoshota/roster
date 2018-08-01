#include "common.h"
#include <QPainter>

Roster::Roster()
    : icon(nullptr)
{
}

Roster::~Roster()
{
    if (icon != nullptr)
        delete icon;
}

QIcon *Roster::icon32x32() const
{
    if (icon == nullptr)
        icon = new QIcon(createPixmap(QSize(32, 32), 0xFFFFFF)); // white
    return icon;
}

QPixmap Roster::createPixmap(const QSize iconSize, QColor bkgndColor) const
{
    QString initials;
    {
        initials.push_back(account.firstName.isEmpty() ? QChar('-') : account.firstName[0]);
        initials.push_back(account.lastName.isEmpty() ? QChar('-') : account.lastName[0]);
    }

    QPixmap pixMap(iconSize);
    QPainter painter(&pixMap);

    pixMap.fill(bkgndColor);

    const QRgb blue = 0xB5E6FF;
    const QRgb pink = 0xFCD0FC;
    const QRgb gray = 0xE1E8ED;

    switch (account.sex)
    {
    case SEX::MALE  : painter.setBrush(QBrush(blue)); break;
    case SEX::FEMALE: painter.setBrush(QBrush(pink)); break;
    default         : painter.setBrush(QBrush(gray)); break;
    }

    QPen pen = painter.pen();
    pen.setStyle(Qt::NoPen);
    painter.setPen(pen);
    painter.drawEllipse(pixMap.rect());

    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawText(pixMap.rect(), Qt::AlignCenter, initials);
    return pixMap;
}
