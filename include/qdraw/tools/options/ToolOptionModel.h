#pragma once

#include <qdraw/tools/options/ToolOption.h>
#include <QAbstractListModel>
#include <QVector>

/// QAbstractListModel exposing a Tool's registered options to QML.
///
/// Populated by DrawingController when the active tool changes.
/// Subscribes to each ToolOption::valueChanged() to emit per-row dataChanged().
///
/// Roles: id (string), label (string), type (int/OptionType),
///        value (var), min (real), max (real), step (real).
/// 'min', 'max', 'step' are meaningful only for RealOption rows.
class ToolOptionModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        TypeRole,
        ValueRole,
        MinRole,
        MaxRole,
        StepRole
    };
    Q_ENUM(Role)

    explicit ToolOptionModel(QObject* parent = nullptr);
    ~ToolOptionModel() override = default;

    /// Replace the displayed option list with the given tool's options.
    /// Disconnects from the previous tool's valueChanged() signals before connecting
    /// to the new tool's options. Emits beginResetModel/endResetModel once.
    void populate(const QVector<ToolOption*>& options);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private slots:
    void onOptionValueChanged();

private:
    QVector<ToolOption*> m_options;
};
