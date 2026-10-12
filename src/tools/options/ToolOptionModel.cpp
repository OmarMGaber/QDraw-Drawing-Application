#include <qdraw/tools/options/ToolOptionModel.h>
#include <qdraw/tools/options/RealOption.h>

ToolOptionModel::ToolOptionModel(QObject* parent)
    : QAbstractListModel(parent) {}

void ToolOptionModel::populate(const QVector<ToolOption*>& options) {
    for (ToolOption* opt : m_options) {
        if (opt) {
            disconnect(opt, &ToolOption::valueChanged, this, &ToolOptionModel::onOptionValueChanged);
        }
    }

    beginResetModel();
    m_options = options;
    endResetModel();

    for (ToolOption* opt : m_options) {
        if (opt) {
            connect(opt, &ToolOption::valueChanged, this, &ToolOptionModel::onOptionValueChanged);
        }
    }
}

int ToolOptionModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_options.size());
}

QVariant ToolOptionModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_options.size()) {
        return {};
    }

    ToolOption* opt = m_options[index.row()];
    if (!opt) {
        return {};
    }

    switch (role) {
    case IdRole:
        return opt->id();
    case LabelRole:
        return opt->label();
    case TypeRole:
        return static_cast<int>(opt->type());
    case ValueRole:
        return opt->value();
    case MinRole: {
        auto* realOpt = qobject_cast<RealOption*>(opt);
        return realOpt ? realOpt->min() : 0.0;
    }
    case MaxRole: {
        auto* realOpt = qobject_cast<RealOption*>(opt);
        return realOpt ? realOpt->max() : 1.0;
    }
    case StepRole: {
        auto* realOpt = qobject_cast<RealOption*>(opt);
        return realOpt ? realOpt->step() : 0.1;
    }
    default:
        return {};
    }
}

QHash<int, QByteArray> ToolOptionModel::roleNames() const {
    return {
        { IdRole, "optionId" },
        { LabelRole, "label" },
        { TypeRole, "type" },
        { ValueRole, "value" },
        { MinRole, "min" },
        { MaxRole, "max" },
        { StepRole, "step" }
    };
}

void ToolOptionModel::onOptionValueChanged() {
    auto* senderOpt = qobject_cast<ToolOption*>(sender());
    if (!senderOpt) {
        return;
    }

    int row = m_options.indexOf(senderOpt);
    if (row >= 0) {
        QModelIndex idx = index(row, 0);
        emit dataChanged(idx, idx, { ValueRole });
    }
}
