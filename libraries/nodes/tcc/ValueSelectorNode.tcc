////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     ValueSelectorNode.tcc (nodes)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ell
{
namespace nodes
{
    template <typename ValueType>
    ValueSelectorNode<ValueType>::ValueSelectorNode()
        : Node({ &_condition, &_input1, &_input2 }, { &_output }), _condition(this, {}, conditionPortName), _input1(this, {}, input1PortName), _input2(this, {}, input2PortName), _output(this, outputPortName, 0)
    {
    }

    template <typename ValueType>
    ValueSelectorNode<ValueType>::ValueSelectorNode(const model::PortElements<bool>& condition, const model::PortElements<ValueType>& input1, const model::PortElements<ValueType>& input2)
        : Node({ &_condition, &_input1, &_input2 }, { &_output }), _condition(this, condition, conditionPortName), _input1(this, input1, input1PortName), _input2(this, input2, input2PortName), _output(this, outputPortName, input1.Size())
    {
        if (condition.Size() != 1)
        {
            throw std::runtime_error("Error: Condition must be 1-D signal");
        }

        if (input1.Size() != input2.Size())
        {
            throw std::runtime_error("Error: input values must be same dimension");
        }
    };

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::Compute() const
    {
        bool cond = _condition[0];
        _output.SetOutput(cond ? _input1.GetValue() : _input2.GetValue());
    };

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::WriteToArchive(utilities::Archiver& archiver) const
    {
        Node::WriteToArchive(archiver);
        archiver[input1PortName] << _input1;
        archiver[input2PortName] << _input2;
        archiver[conditionPortName] << _condition;
    }

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::ReadFromArchive(utilities::Unarchiver& archiver)
    {
        Node::ReadFromArchive(archiver);
        archiver[input1PortName] >> _input1;
        archiver[input2PortName] >> _input2;
        archiver[conditionPortName] >> _condition;
        _output.SetSize(_input1.Size());
    }

    template <typename ValueType>
    void ValueSelectorNode<ValueType>::Copy(model::ModelTransformer& transformer) const
    {
        auto newCondition = transformer.TransformPortElements(_condition.GetPortElements());
        auto newPortElements1 = transformer.TransformPortElements(_input1.GetPortElements());
        auto newPortElements2 = transformer.TransformPortElements(_input2.GetPortElements());

        auto newNode = transformer.AddNode<ValueSelectorNode<ValueType>>(newCondition, newPortElements1, newPortElements2);

        transformer.MapNodeOutput(output, newNode->output);
    }
}
}
