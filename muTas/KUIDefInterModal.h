#pragma once
namespace UIInterModal
{

//백그라운드
const CString
    UIXAML_D_BACKIMAGE
        (
        _T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#8e8269' Offset='0'/>")
        _T("      <GradientStop Color='#cbc1b4' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")
        _T("</Border>")
        );


//메인모델 (INTERMODAL)
const CString
	UIXAML_D_MODEL_DEFAULT 
	(
	_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='13'>")       
	_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90915'/></TextBlock>")    
	_T("    <Border Height='1' Background='#ddc09e' />")
	_T("    <Border Height='1' Background='#ffffff' />")
	_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> </TextBlock>")  
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal </TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_MODEL_SELECTED 
	(
	_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='3,3,3,3'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='13'>")       
	_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90915'/> </TextBlock>")    
	_T("    <Border Height='1' Background='#7c96b5' />")
	_T("    <Border Height='1' Background='#9acae4' />")
	_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> </TextBlock>")  
    _T("    <TextBlock Padding='5, 10, 5, 1' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal </TextBlock>")    
	//_T("    <TextBlock Padding='15, 5, 5, 10' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Intermodal</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_PURPOSE_INPUT_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Input(Tijp)</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_PURPOSE_INPUT_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Input(Tijp)</TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_PURPOSE_INPUT_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Input(Tijp)</TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);
const CString
	UIXAML_D_MODE_INPUT_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Trip Matrix </TextBlock>")   
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_MODE_INPUT_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Trip Matrix </TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_MODE_INPUT_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Trip Matrix </TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
    UIXAML_D_DEFINECOST_FALSE
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
    _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Define Cost</TextBlock>")    
    //_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Run Options</TextBlock>")   
    _T("  </StackPanel>")
    _T("</Border>")
    );

const CString
    UIXAML_D_DEFINECOST_DEFAULT
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
    _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Define Cost</TextBlock>")    
  //  _T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333' HorizontalAlignment='Center'> Run Options</TextBlock>")  
    _T("  </StackPanel>")
    _T("</Border>")
    );

const CString
    UIXAML_D_DEFINECOST_SELECTED 
    (
    _T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
    _T("  <Border.Background>")
    _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
    _T("      <GradientStop Color='#378BBA' Offset='0'/>")
    _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
    _T("    </LinearGradientBrush>")
    _T("  </Border.Background>")        
    _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
    _T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Define Cost</TextBlock>")    
    _T("  </StackPanel>")
    _T("</Border>")
    );



const CString
	UIXAML_D_DEFINEVDF_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Volume Delay </TextBlock>")    
	_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_DEFINEVDF_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Volume Delay </TextBlock>")
	_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_DEFINEVDF_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Volume Delay </TextBlock>")
	_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366' HorizontalAlignment='Center'> Function</TextBlock>")   
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_TRANASSIGN_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Transit Assign </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Options </TextBlock>")        
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_TRANASSIGN_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Transit Assign </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Options </TextBlock>")         
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_TRANASSIGN_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Transit Assign </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Options </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);



const CString
	UIXAML_D_CHOICEPARAM_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Mode/Route Choice </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Parameter </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_CHOICEPARAM_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Mode/Route Choice </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Parameter </TextBlock>")   
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_CHOICEPARAM_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
	_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Mode/Route Choice </TextBlock>")         
	_T("    <TextBlock Padding='5, 1, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Parameter </TextBlock>")    
	_T("  </StackPanel>")
	_T("</Border>")
	);





const CString
	UIXAML_D_STOPCRITERIA_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> Stopping Criteria </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_STOPCRITERIA_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> Stopping Criteria </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_STOPCRITERIA_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Stopping Criteria </TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_VTABLE_FALSE
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
	_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'> View Table </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_VTABLE_DEFAULT
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
	_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'> View Table </TextBlock>")      
	_T("  </StackPanel>")
	_T("</Border>")
	);

const CString
	UIXAML_D_VTABLE_SELECTED 
	(
	_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
	_T("  <Border.Background>")
	_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
	_T("      <GradientStop Color='#378BBA' Offset='0'/>")
	_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
	_T("    </LinearGradientBrush>")
	_T("  </Border.Background>")        
	_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
	_T("    <TextBlock Padding='5, 10, 5, 5' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> View Table </TextBlock>")     
	_T("  </StackPanel>")
	_T("</Border>")
	);






const CString
    UIXAML_D_OUTPUT_FALSE
        (
        _T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#9da3a7' Offset='0'/>")
        _T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/>  </TextBlock>")    
        _T("    <Border Height='1' Background='#a0a5a8' />")
        _T("    <Border Height='1' Background='#cdd1d4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")    
        _T("  </StackPanel>")
        _T("</Border>")
        );

const CString
    UIXAML_D_OUTPUT_DEFAULT 
        (
        _T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#faf9e1' Offset='0'/>")
        _T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#ddc09e' />")
        _T("    <Border Height='1' Background='#ffffff' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );

const CString
        UIXAML_D_OUTPUT_SELECTED 
        (
        _T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
        _T("  <Border.Background>")
        _T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
        _T("      <GradientStop Color='#378BBA' Offset='0'/>")
        _T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
        _T("    </LinearGradientBrush>")
        _T("  </Border.Background>")        
        _T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
        _T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90917'/> </TextBlock>")    
        _T("    <Border Height='1' Background='#7c96b5' />")
        _T("    <Border Height='1' Background='#9acae4' />")
        _T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
        _T("  </StackPanel>")
        _T("</Border>")
        );
}