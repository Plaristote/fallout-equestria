#ifndef  FUNCTORTHREAD_H
# define FUNCTORTHREAD_H

# include <QThread>
# include <QMutex>

class FunctorThread : public QThread
{
    Q_OBJECT
public:
    static FunctorThread* Create(std::function<void (void)> proc)
    {
      return (new FunctorThread(proc));
    }

    ~FunctorThread(void)
    {
      mutex.lock();
      functor_threads.removeOne(this);
      mutex.unlock();
    }

    void SetProgress(float value)
    {
      emit UpdateProgressBar(value);
    }

    void start_sync(void)
    {
        run();
    }

signals:
    void Done(void);
    void UpdateProgressBarPlease(void);
    void UpdateProgressBar(float value);
    void DeletePlease(void);

private slots:
    void ExecDeletePlease(void)
    {
      delete this;
    }

private:
    FunctorThread(std::function<void (void)> proc) : proc(proc)
    {
      mutex.lock();
      functor_threads.push_back(this);
      mutex.unlock();
      connect(this, SIGNAL(DeletePlease()), this, SLOT(ExecDeletePlease()), Qt::QueuedConnection);
    }

    void run(void)
    {
      proc();
      Done();
      DeletePlease();
    }

    std::function<void (void)> proc;

    static QMutex                mutex;
    static QList<FunctorThread*> functor_threads;
};


#endif // FUNCTORTHREAD_H
