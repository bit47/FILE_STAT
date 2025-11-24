import matplotlib.pyplot as plt

plt.rcParams["font.family"] = ["SimHei", "WenQuanYi Micro Hei", "Heiti TC"]
plt.rcParams["axes.unicode_minus"] = False 

words = []
counts = []

with open('test/result.txt', 'r', encoding='utf-8') as f:
    lines = f.readlines()

    for i in range(4, len(lines)):
        line = lines[i].strip()
        if not line:
            continue  
        
        # 按“:”分割单词和次数
        parts = line.split(':')
        if len(parts) != 2:
            continue
        
        word = parts[0].strip() 
        count_str = parts[1].strip()  
        count = int(count_str)  
        
        words.append(word)
        counts.append(count)

if not words or not counts:
    print("empty result, no data to plot.")
else:
    # 绘制柱状图
    plt.bar(words, counts, 
            color=['red', 'green', 'blue', 'yellow', 'purple'])
    plt.title('高频单词统计')
    plt.xlabel('单词')
    plt.ylabel('出现次数')
    plt.xticks(rotation=45, ha='right')  # 旋转x轴标签避免重叠
    plt.tight_layout()  # 自动调整布局
    plt.savefig('test/top5.png', dpi=300, bbox_inches='tight')  # 保存高清晰度图片
   
plt.show()