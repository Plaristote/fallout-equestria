#include "dialogeditor.h"
#include "ui_dialogeditor.h"
#include <QInputDialog>
#include <QMessageBox>

DialogEditor::DialogEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogEditor)
{
    QIcon iconDelete("icons/delete.png");
    QIcon iconAdd("icons/add.png");
    QIcon iconUp("icons/up.png");
    QIcon iconDown("icons/down.png");

    tree = nullptr;
    ui->setupUi(this);

    ui->nodeRemove->setIcon(iconDelete);
    ui->nodeNew->setIcon(iconAdd);
    ui->successorAdd->setIcon(iconAdd);
    ui->successorRemove->setIcon(iconDelete);
    ui->successorUp->setIcon(iconUp);
    ui->successorDown->setIcon(iconDown);

	//Make the first column of the successor list non-editable
	ui->successorList->setItemDelegateForColumn(0, new NoEditDelegate(this));

    connect(ui->nodeRemove,      SIGNAL(clicked()), this, SLOT(DeleteNode()));
    connect(ui->nodeNew,         SIGNAL(clicked()), this, SLOT(NewNode()));
    connect(ui->successorAdd,    SIGNAL(clicked()), this, SLOT(NewSuccessor()));
    connect(ui->successorRemove, SIGNAL(clicked()), this, SLOT(DeleteSuccessor()));
    connect(ui->successorDown,   SIGNAL(clicked()), this, SLOT(MoveDown()));
    connect(ui->successorUp,     SIGNAL(clicked()), this, SLOT(MoveUp()));
    connect(ui->nodeList,        SIGNAL(currentTextChanged(QString)), this, SLOT(SwapNode(QString)));
    connect(ui->successorList,   SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(SuccessorChanged(QTreeWidgetItem*,int)));
	connect(ui->successorList,   SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(DoubleclickSuccessor(QTreeWidgetItem*,int)));
    connect(ui->successorList,   SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(ChangedCurrentSuccessor()));
	connect(ui->npcLine,         SIGNAL(textEdited(QString)), this, SLOT(NpcLineChanging()));
    connect(ui->npcLine,         SIGNAL(editingFinished()), this, SLOT(NpcLineChanged()));
	connect(ui->localePreview,   SIGNAL(textChanged()), this, SLOT(NpcLineLocaleChanged()));

	nodeDoc= ui->localePreview->document();
}


DialogEditor::~DialogEditor()
{
    delete ui;
}

void DialogEditor::ChangedCurrentSuccessor()
{
}

void DialogEditor::DoubleclickSuccessor(QTreeWidgetItem* item,int column)
{
	if (column==0) {
		std::string target= item->text(1).toStdString();
		Data		dialogue(tree);
		QString		clickedOn= item->text(0);
		
		//Do different things, depending on which subnode was actually clicked
		if ( clickedOn == "Goto") {
			//If the user double-clicked on the "Goto" sub-category, go to the new node!

			//Check if the target node actually exists
			if ( !dialogue[target].Nil() ) {
				//WE GO!
				//TODO: History
				//NB: We do NOT call SwapNode(), because we also want the list widget to update to the new selection
				QList<QListWidgetItem*> finds= ui->nodeList->findItems( QString::fromStdString(target), Qt::MatchFixedString );
				if (!finds.empty())
					ui->nodeList->setCurrentItem( finds[0] );
			} else {
				//If it doesn't exist, and it isn't a blank string, we ask for it to be created
				if ( !target.empty() ) {
					//std::cout<< "MAEK TEH " << target << std::endl;;
					 QMessageBox msgBox;
					 msgBox.setText(  QString::fromStdString("The node '" + target + "' does not exist")  );
					 msgBox.setInformativeText("Do you wish to create it?");
					 msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
					 msgBox.setDefaultButton(QMessageBox::Yes);
					 int ret = msgBox.exec();
					 if (ret==QMessageBox::Yes)
						 AddNode( QString::fromStdString(target) );

				};
			};
		} else if (clickedOn == "HookExec") {
			//TODO: Jump to the hook in the script editor
			QMessageBox::warning(this, "Error", "Lazy programmer detected (A)");

		} else if (clickedOn == "HookShow") {
			//TODO: Jump to the hook in the script editor
			QMessageBox::warning(this, "Error", "Lazy programmer detected (B)");

		} else if (clickedOn.startsWith("#")) {
			//Open a successor renaming dialogue!
			QString oldname= clickedOn.remove(0,1);
			QString newname = QInputDialog::getText(this, "Rename successor", "Name", QLineEdit::Normal, oldname);

			if (newname!="") {
				item->setText(0,"#"+newname);
				item->setText(1, I18n( newname.toStdString() ) );
				dialogue[ui->nodeList->currentItem()->text().toStdString()][oldname.toStdString()].SetKey(newname.toStdString());
				SaveData();
			};

		};
	};
};

//A graphical notice, to make it obvious the preview document is stale
void DialogEditor::NpcLineChanging()
{
	ui->localePreview->setEnabled(false);
}

//Only when editing of the line is finished does this method get called!
void DialogEditor::NpcLineChanged()
{
	//Sanity check 1: Current node does not exist
	if (!ui->nodeList->currentItem()) {
		nodeDoc->clear();
		ui->npcLine->clear();
		ui->localePreview->setEnabled(false);
		return;
	};

	//Sanity check 2: Text box is empty
	if ( !(ui->npcLine->text().startsWith("#")) ) {
		ui->npcLine->setText("#");
		return;
	};

	//Sanity check 3: Textbox only contains the '#' character
	if ( ui->npcLine->text()=="#" ) {
		return;
	};

	//Re-enable the locale doc
	ui->localePreview->setEnabled(true);

    QString newLine = ui->npcLine->text().remove(0,1); //Get rid of the '#' prefix
    Data    data(tree);
	Data    loc(locale);
	Data    node    = data[ui->nodeList->currentItem()->text().toStdString()];

    node["npcLine"] = newLine.toStdString();
	SaveData();

	//Update the document below; either clear it completely, or load a new localization
	if (loc[ newLine.toStdString() ].Nil())
		nodeDoc->clear();
	else
		nodeDoc->setPlainText( I18n( node["npcLine"].Value() ) );
}

//Save the new locale text
void DialogEditor::NpcLineLocaleChanged()
{
	//Sanity check first: Does the current node exist?
	if ( ui->nodeList->currentItem() ) {
		Data    data(tree);
		Data    node = data[ ui->nodeList->currentItem()->text().toStdString()];
		Data	loc(locale);

		if (node.Nil())
			return;

		if (node["npcLine"].Value()=="")
			return;

		loc[   node["npcLine"].Value() ]= nodeDoc->toPlainText().toStdString();

		SaveData(true);
	};
};

void DialogEditor::SuccessorChanged(QTreeWidgetItem* item, int column)
{
	//First column is ignored, as it is non-editable anyway
	if (column==0)
		return;
    {
        Data        data(tree);
		Data		loc(locale);
		QString		row= item->text(0);

		if (row.isEmpty())
			return;

		//If we're on the top node, we'll have to change the localization!!!
		if (row.startsWith("#")) {
			if (!item->text(1).isEmpty()) {
				QString key= row.remove(0,1); //Get rid of the '#' prefix;

				loc[ key.toStdString() ]= item->text(1).toStdString();
				SaveData(true);
			};
		} else {
			//Otherwise, we're simply changing just the dialogue file
			//Also, we're on the bottom node, so we have to get the key in a slightly more complicated way
			QString qkey= item->parent()->text(0).remove(0,1);
			Data        node          = data[ui->nodeList->currentItem()->text().toStdString()];
			Data        successor     = node[ qkey.toStdString() ];
			//successor.SetKey( qkey.toStdString() );

			if (row=="HookShow")
				successor["HookAvailable"]= item->text(1).toStdString();
			if (row=="HookExec")
				successor["HookExecute"]= item->text(1).toStdString();
			if (row=="Goto")
				successor["DefaultAnswer"]= item->text(1).toStdString();

			SaveData();

		};
    }
}

//Swap current node
//Do not call directly from code! Otherwise, listwidget won't be updated
void DialogEditor::SwapNode(QString str)
{
	//Sanity check 1: Empty parameter + does the current node exist?
	if ((str=="") || (!ui->nodeList->currentItem())) {
		ui->frame->setEnabled(false);
		return;
	} else {
		ui->frame->setEnabled(true);
	};

    Data           data(tree);
    Data           node(data[str.toStdString()]);
    Data::iterator it  = node.begin();
    Data::iterator end = node.end();

	//Next, reset the locale doc
	nodeDoc->clearUndoRedoStacks();

	//Finally, generate the successors list
    ui->successorList->clear();
    for (; it != end ; ++it)
    {
		if ( (*it).Nil() )
			continue;
        if ((*it).Key() == "npcLine") {
			continue;
		};

		auto a= (*it).Key();

		AddSuccessor( str.toStdString(), (*it).Key() );

    }
	ui->npcLine->setText(QString::fromStdString("#" + node["npcLine"].Value()));
	NpcLineChanged();
}

//Set up the widgets/etc. for a new successor
void DialogEditor::AddSuccessor(std::string node, std::string key)
{
	Data           data(tree);
	Data		   successor( data[node][key] );


	QString name          = QString::fromStdString( "#" + key );
    QString hookAvailable = QString::fromStdString( successor["HookAvailable"] );
    QString hookExecute   = QString::fromStdString( successor["HookExecute"] );
    QString defaultAnswer = QString::fromStdString( successor["DefaultAnswer"] );

    QTreeWidgetItem* item = new QTreeWidgetItem(ui->successorList);
	item->setExpanded(true);
	item->setText(0, name );
	item->setFlags (item->flags () | Qt::ItemIsEditable);
	item->setText(1, I18n( key ) );
	QTreeWidgetItem* subhookshow = new QTreeWidgetItem(item);
	subhookshow->setText(0, "HookShow");
	subhookshow->setText(1, hookAvailable);
	subhookshow->setFlags (item->flags () | Qt::ItemIsEditable);
	QTreeWidgetItem* subhookexec = new QTreeWidgetItem(item);
	subhookexec->setText(0, "HookExec");
	subhookexec->setText(1, hookExecute);
	subhookexec->setFlags (item->flags () | Qt::ItemIsEditable);
	QTreeWidgetItem* subanswer = new QTreeWidgetItem(item);
	subanswer->setText(0, "Goto");
	subanswer->setText(1, defaultAnswer);
	subanswer->setFlags (item->flags () | Qt::ItemIsEditable);

	SaveData();
};

//Scan a DataTree, return a suitable+unique integer key for it
//Using QStrings so that I don't have to re-implement string<->int functions
//Seriously, you'd THINK they'd be in the standard library somewhere, wouldn't you?
//This is like the one annoying thing about C++
std::string  DialogEditor::GetIntTag(DataTree* tree)
{
	int					istr;
	Data				data(tree);
	int					max= 10000;
	bool				okay;

	Data::iterator itr= data.begin();
	Data::iterator end= data.end();

	//Scan the tree
	for ( ; itr!=end; ++itr) {
		//Attempt a conversion of the current key to an integer
		istr= QString::fromStdString( (*itr).Key() ).toInt(&okay);

		//If the conversion succeeded, and the new key is bigger than the last biggest one, save it!
		if (okay)
			if (istr>max)
				max= istr;
	};

	//Don't forget that 'plus one' there. Unlike a certain idiot
	QString str= QString::number(max+1);

	return str.toStdString();
};

void DialogEditor::LoadDialog(DataTree* tree, DataTree* locale)
{
    Data           data(tree);
    Data::iterator it  = data.begin();
    Data::iterator end = data.end();

    this->tree = tree;
	this->locale= locale;
    ui->nodeList->clear();
    ui->successorList->clear();
	ui->npcLine->clear();
	ui->localePreview->clear();
    for (; it != end ; ++it)
    {
		if ( !((*it).Nil()) )
			AddNode( QString::fromStdString((*it).Key()) );
    }
    this->setEnabled(true);
}

//Set up the widgets/etc. for a new node
//Do not call directly from inteface (unlike a certain idiot)
void DialogEditor::AddNode(QString name)
{
	//Sanity check 1: Do not add a blank node
	if (name=="")
		return;

    Data data(tree);

	//If this node does not yet have an npcLine (OR it doesn't exist at all), generate it!
	if (data[name.toStdString()]["npcLine"].Nil())
		data[name.toStdString()]["npcLine"]= GetIntTag(locale);

	QListWidgetItem* nuitem= new QListWidgetItem(name);
	ui->nodeList->addItem(nuitem);

	//Go to the new node
	ui->nodeList->setCurrentItem( nuitem );

    SaveData();
}

void DialogEditor::DeleteNode(void)
{
	{
		if (ui->nodeList->count() > 0)
		{
			int ret = QMessageBox::warning(this, "Warning", "Permanently delete node?", QMessageBox::Yes, QMessageBox::No);
			if (ret==QMessageBox::Yes) {

				Data data(tree);
				data[ui->nodeList->currentItem()->text().toStdString()].Remove();

				//The destructor updates the parent widget automatically, no need to worry
				delete ui->nodeList->currentItem();
			
				//Swap to the last still-existing node
				ui->nodeList->setCurrentItem( ui->nodeList->item( ui->nodeList->count()-1 ) );
			};
		}
	}
	SaveData();
}

void DialogEditor::SaveData(bool save_I18n)
{
	//Save the dialogue file!
    DataTree::Writers::JSON(tree, tree->GetSourceFile());
	//Save the internationalization! (if we have to)
	if (save_I18n) {
		DataTree::Writers::JSON(locale, locale->GetSourceFile());

		//TODO: Make formI18n reload the current line! To maintain consistency and so on
		UpdateLocale();
	};
}

//Add a node with the NEW button
void DialogEditor::NewNode(void)
{
    QString name = QInputDialog::getText(this, "New dialog node", "Name");

    if (name != "")
    {
        Data data(tree);

        if (data[name.toStdString()].Nil()) {
			//Create a new node
			AddNode(name);
			//Blank the preview doc
			ui->localePreview->clear();
		}
        else
          QMessageBox::warning(this, "Error", "A dialog node with that name already exist");
    }
}

//Add a successor with the NEW button
void DialogEditor::NewSuccessor(void)
{
	AddSuccessor( ui->nodeList->currentItem()->text().toStdString() , GetIntTag(locale));
}

void DialogEditor::DeleteSuccessor(void)
{
   QTreeWidgetItem* item = ui->successorList->currentItem();
   {
		if (item) {
		  QString        itemName = item->text(0);

		  //If we're on the top row, simply delete the whole key
		  //Else, delete the parent (after saving its name)
		  if (itemName.startsWith("#"))
			  delete item;
		  else {
			  itemName= item->parent()->text(0);
			  delete item->parent();
		  };

		  Data           data(tree);
		  std::string successorName= itemName.remove(0,1).toStdString();
		  data[ui->nodeList->currentItem()->text().toStdString()][successorName].Remove();
		};
	}
	SaveData();
}

void DialogEditor::MoveUp(void)
{
	QMessageBox::warning(this, "Error", "Lazy programmer detected (C)");
    /*QTreeWidgetItem* item = ui->successorList->currentItem();
    int              row  = ui->successorList->currentIndex().row();

    if (item && row > 0)
    {
        Data data(tree);

        ui->successorList->takeTopLevelItem(row);
        ui->successorList->insertTopLevelItem(row - 1, item);
        ui->successorList->setCurrentItem(item);
		data[ui->nodeList->currentItem()->text().toStdString()][item->text(0).toStdString()].MoveUp();
        SaveData();
    }*/
}

void DialogEditor::MoveDown(void)
{
	QMessageBox::warning(this, "Error", "Lazy programmer detected (D)");
    /*QTreeWidgetItem* item = ui->successorList->currentItem();
    QTreeWidgetItem* below = ui->successorList->itemBelow(item);

    if (item && below)
    {
        Data data(tree);

        int row = ui->successorList->currentIndex().row();
        ui->successorList->takeTopLevelItem(row);
        ui->successorList->insertTopLevelItem(row + 1, item);
        ui->successorList->setCurrentItem(item);
        data[ui->nodeList->currentItem()->text().toStdString()][item->text(0).toStdString()].MoveDown();
        SaveData();
    }*/
}
