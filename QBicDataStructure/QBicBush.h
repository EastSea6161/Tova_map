#pragma once

template <typename Type1>
class QBicBush : public std::enable_shared_from_this<QBicBush<Type1>>
{
public:
    typedef std::shared_ptr<QBicBush<Type1>> QBicBushNodePtr;
public:
    QBicBush(Type1 BushID) : TxBushID(BushID) {        
    }

    virtual ~QBicBush() {
    }

public: 
    Type1 TxBushID;
public:
    struct BushCompare  {
        bool operator () ( QBicBushNodePtr spLeft, QBicBushNodePtr spRight ) const
        {
            return spLeft->TxBushID < spRight->TxBushID;
        }
    };

protected:
    std::set<QBicBushNodePtr, BushCompare> m_setChild;

public:
    virtual void Plus(QBicBushNodePtr spRight) = 0;

protected: 
    virtual QBicBushNodePtr AddChild(QBicBushNodePtr spChild) 
    {
        auto iter = m_setChild.find(spChild);
        if (iter == m_setChild.end()) {
            m_setChild.insert(spChild);
            return spChild;
        }

        //★ 신규 경로
        QBicBushNodePtr spNode = *iter;
        spNode->Plus(spChild);
        return spNode;
    }

public:
    virtual void InsertPath(std::vector<QBicBushNodePtr>& vecPath) 
    {
        QBicBushNodePtr spParent = shared_from_this();

        for (size_t i = 0; i < vecPath.size(); i++) {
            QBicBushNodePtr spChild = vecPath[i];
            spParent = spParent->AddChild(spChild);       
        }
    }

private:
    virtual int ChildCount(QBicBushNodePtr spNode) 
    {
        int nCount(0); {
            std::set<QBicBushNodePtr, BushCompare>& setChild = spNode->m_setChild;
            nCount = (int)setChild.size();
        }
        
        return nCount;
    }

private:
    virtual int NodeCount(QBicBushNodePtr spNode) 
    {
        std::set<QBicBushNodePtr, BushCompare>& setChild = spNode->m_setChild;
        int nCount(1);
        for (auto iter = setChild.begin(); iter != setChild.end(); ++iter) {
            QBicBushNodePtr spNode = *iter;
            nCount += NodeCount(spNode);
        }

        return nCount;
    }

    virtual int PathCount(QBicBushNodePtr spNode) 
    {
        int nCount(0);
        std::set<QBicBushNodePtr, BushCompare>& setChild = spNode->m_setChild;
        
        for (auto iter = setChild.begin(); iter != setChild.end(); ++iter) {
            QBicBushNodePtr spNode = *iter;
            nCount += PathCount(spNode);
        }

        //★ 자식이 없는 경우, 마지막.. (재귀)
        if (setChild.size() == 0)
            return 1;

        return nCount;
    }

public:
    virtual int PathCount() 
    {
        if (m_setChild.size() == 0)
            return 0;

        QBicBushNodePtr spParent = shared_from_this();
        int nCount = PathCount(spParent);

        return nCount;
    }

private:
    virtual int Path(QBicBushNodePtr spNode, int nParKey, std::map<int, std::vector<QBicBushNodePtr>>& mapPath) 
    {
        //★ nKey : 현재 Path의 키값
        int nKey(nParKey);
        std::set<QBicBushNodePtr, BushCompare>& setChild = spNode->m_setChild;

        std::vector<QBicBushNodePtr> vecPrePath = mapPath[nKey];
        for (auto iter = setChild.begin(); iter != setChild.end(); ++iter) {
            QBicBushNodePtr spNode = *iter;
            std::vector<QBicBushNodePtr>& vecPath = mapPath[nKey]; {
                if (vecPath.size() == 0) {
                    vecPath = vecPrePath;
                }

                vecPath.push_back(spNode);
            }

            nKey = Path(spNode, nKey, mapPath);
            nKey++; //★ 무조건 증가시키자..          
        }

        return nKey;
    }

public:
    virtual void Path(std::map<int, std::vector<QBicBushNodePtr>>& mapPath) 
    {
        mapPath.clear();

        QBicBushNodePtr spParent = shared_from_this();
        std::map<int, std::vector<QBicBushNodePtr>> mapTempPath;
        Path(spParent, 0, mapTempPath);

        int nKey(0);
        for (auto iter = mapTempPath.begin(); iter != mapTempPath.end(); ++iter) {
            std::vector<QBicBushNodePtr>& vecPath = iter->second;
            if (vecPath.size() == 0)
                continue;

            mapPath[nKey] = vecPath;
            nKey++;
        }
    }
};
