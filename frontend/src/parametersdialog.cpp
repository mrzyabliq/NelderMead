#include "parametersdialog.h"

ParametersDialog::ParametersDialog(QWidget *parent)
    : QDialog(parent),
    alphaEdit(new QLineEdit("1.0", this)),
    betaEdit(new QLineEdit("0.5", this)),
    gammaEdit(new QLineEdit("2.0", this)),
    sigmaEdit(new QLineEdit("1.0", this)),
    toleranceEdit(new QLineEdit("0.000001", this)),
    startPointEdit(new QLineEdit(this))
{
    QFormLayout *layout = new QFormLayout(this);

    // Настройка валидаторов
    QDoubleValidator *validator = new QDoubleValidator(this);
    alphaEdit->setValidator(validator);
    betaEdit->setValidator(validator);
    gammaEdit->setValidator(validator);
    sigmaEdit->setValidator(validator);
    toleranceEdit->setValidator(validator);

    // Добавление элементов
    layout->addRow("Alpha (отражение):", alphaEdit);
    layout->addRow("Beta (сжатие):", betaEdit);
    layout->addRow("Gamma (растяжение):", gammaEdit);
    layout->addRow("Sigma (редукция):", sigmaEdit);
    layout->addRow("Точность:", toleranceEdit);
    layout->addRow("Начальная точка:", startPointEdit);

    // Кнопки
    QPushButton *resetBtn = new QPushButton("Сбросить", this);
    QPushButton *okBtn = new QPushButton("OK", this);
    QPushButton *cancelBtn = new QPushButton("Отмена", this);

    connect(resetBtn, &QPushButton::clicked, this, &ParametersDialog::resetToDefaults);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    layout->addRow(resetBtn);
    layout->addRow(okBtn, cancelBtn);
}

// Реализация геттеров
double ParametersDialog::getAlpha() const { return alphaEdit->text().toDouble(); }
double ParametersDialog::getBeta() const { return betaEdit->text().toDouble(); }
double ParametersDialog::getGamma() const { return gammaEdit->text().toDouble(); }
double ParametersDialog::getSigma() const { return sigmaEdit->text().toDouble(); }
double ParametersDialog::getTolerance() const { return toleranceEdit->text().toDouble(); }
QString ParametersDialog::getStartPoint() const { return startPointEdit->text(); }

// Реализация сеттеров
void ParametersDialog::setAlpha(double value) { alphaEdit->setText(QString::number(value)); }
void ParametersDialog::setBeta(double value) { betaEdit->setText(QString::number(value)); }
void ParametersDialog::setGamma(double value) { gammaEdit->setText(QString::number(value)); }
void ParametersDialog::setSigma(double value) { sigmaEdit->setText(QString::number(value)); }
void ParametersDialog::setTolerance(double value) { toleranceEdit->setText(QString::number(value)); }
void ParametersDialog::setStartPoint(const QString& value) { startPointEdit->setText(value); }

void ParametersDialog::resetToDefaults()
{
    setAlpha(1.0);
    setBeta(0.5);
    setGamma(2.0);
    setSigma(1.0);
    setTolerance(0.000001);
    setStartPoint("");
}
