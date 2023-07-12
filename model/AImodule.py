import torch
import numpy as np
from nerpy import NERModel
from transformers import AutoModelForTokenClassification, TrainingArguments, Trainer
import timeparser as tp
import json


def get_token(input):
    # english = 'abcdefghijklmnopqrstuvwxyz0123456789'
    english = 'abcdefghijklmnopqrstuvwxyz'
    output = []
    buffer = ''
    for s in input:
        if s in english or s in english.upper():
            buffer += s
        else:
            if buffer: output.append(buffer)
            buffer = ''
            output.append(s)
    if buffer: output.append(buffer)
    return output


def get_type(input_str):
    model = AutoModelForTokenClassification.from_pretrained('./output/checkpoint-50000')
    print(model)
    print(model.config.id2label)

    from transformers import BertTokenizerFast

    tokenizer = BertTokenizerFast.from_pretrained('bert-base-chinese')

    input_char = get_token(input_str)
    input_tensor = tokenizer(input_char, is_split_into_words=True, padding=True, truncation=True,
                             return_offsets_mapping=True, max_length=512, return_tensors="pt")
    input_tokens = input_tensor.tokens()
    offsets = input_tensor["offset_mapping"]
    ignore_mask = offsets[0, :, 1] == 0
    # print(input_tensor)
    input_tensor.pop("offset_mapping")  # 不剔除的话会报错
    outputs = model(**input_tensor)
    probabilities = torch.nn.functional.softmax(outputs.logits, dim=-1)[0].tolist()
    predictions = outputs.logits.argmax(dim=-1)[0].tolist()
    print(predictions)
    results = []

    tokens = input_tensor.tokens()
    idx = 0
    while idx < len(predictions):
        if ignore_mask[idx]:
            idx += 1
            continue
        pred = predictions[idx]
        label = model.config.id2label[pred]
        if label != "O":
            # 不加B-或者I-
            label = label[2:]
            start = idx
            end = start + 1
            # 获取所有token I-label
            all_scores = []
            all_scores.append(probabilities[start][predictions[start]])
            while (
                    end < len(predictions)
                    and model.config.id2label[predictions[end]] == f"I-{label}"
            ):
                all_scores.append(probabilities[end][predictions[end]])
                end += 1
                idx += 1
            # 得到是他们平均的
            score = np.mean(all_scores).item()
            word = input_tokens[start:end]
            # 重新组织results字符串
            # results.append(
            #     {
            #         "entity_group": label,
            #         "score": score,
            #         "word": word,
            #         "start": start,
            #         "end": end,
            #     }
            # )
            results.append(label)
        idx += 1
    # for i in range(len(results)):
    #     print(results[i])
    print(results)
    return results


def get_entity(sentence):
    model = NERModel("bert", "shibing624/bert4ner-base-chinese")
    predictions, raw_outputs, entities = model.predict([sentence],
                                                       split_on_space=False)
    print("Sentence entity:")
    print(entities)
    return entities


def get_time(sentence):
    res = tp.extract_time(sentence)

    print(res)
    return res


def get_result(input_str):
    input_type = list(set(get_type(input_str)))  # 去重
    input_type_str = ""
    input_entity_list = list()
    # for i in range(len(input_type)):
    #     if input_type_str == "":
    #         input_type_str += input_type[i]
    #     else:
    #         input_type_str += "," + input_type[i]

    input_entity = get_entity(input_str)
    for i in range(len(input_entity)):
        for j in range(len(input_entity[i])):
            if input_entity[i][j][1] == 'TIME':
                continue
            else:
                input_entity_list.append(input_entity[i][j][0])
    input_time = get_time(input_str)  # 提取时间
    result = list()
    result.append({
        "type": input_type,
        "entity": input_entity_list,
        "time": input_time
    })
    resultjson = json.dumps(result, ensure_ascii=False)
    return resultjson


# if __name__ == '__main__':
#     input_str = '我想找一个上周的音乐文件，可以是薛之谦或者周杰伦的'
#     mainResult = get_result(input_str)
#     print(mainResult)
